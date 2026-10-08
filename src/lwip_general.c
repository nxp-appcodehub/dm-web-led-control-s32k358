/*
 * Copyright 2017-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Boot flow:
 *     main.c : device_init()  -> clocks, pins, Ethernet MAC
 *     main.c : lwip_run()     -> this file
 *                 lwip_init()           - start the stack
 *                 interface_init()      - add + bring up the network interface
 *                 start_command_server()- listen on APP_SERVER_PORT (8080)
 *                 poll loop             - sys_check_timeouts + EthIf RX/TX
 */

#include "lwip_general.h"
#include "lwip_all_includes.h"
#include "application.h"

/* Provided by the port / startup code. */
extern void sysarch_init(void);

/* The single network interface used by this demo. */
static struct netif net_if;

/* Listening PCB for the LED command server. */
static struct tcp_pcb *server_pcb;

/* -------------------------------------------------------------------------- */
/* Network interface setup                                                    */
/* -------------------------------------------------------------------------- */

/*
 * Add the network interface with the static IP from the port configuration
 * (netif_cfg[0]) and bring it up. DHCP/AutoIP/IPv6 are intentionally left out
 * to keep the bare-metal example minimal.
 */
static void interface_init(void)
{
    ip4_addr_t ipaddr, netmask, gw;

    ip4_addr_set_zero(&ipaddr);
    ip4_addr_set_zero(&netmask);
    ip4_addr_set_zero(&gw);

    IP4_ADDR(&gw,      netif_cfg[0]->gw[0],      netif_cfg[0]->gw[1],
                       netif_cfg[0]->gw[2],      netif_cfg[0]->gw[3]);
    IP4_ADDR(&ipaddr,  netif_cfg[0]->ip_addr[0], netif_cfg[0]->ip_addr[1],
                       netif_cfg[0]->ip_addr[2], netif_cfg[0]->ip_addr[3]);
    IP4_ADDR(&netmask, netif_cfg[0]->netmask[0], netif_cfg[0]->netmask[1],
                       netif_cfg[0]->netmask[2], netif_cfg[0]->netmask[3]);

    netif_set_default(netif_add(&net_if, &ipaddr, &netmask, &gw, NULL,
                                Tcpip_EthernetifInit, netif_input));
    netif_set_up(&net_if);
}

/* -------------------------------------------------------------------------- */
/* TCP command server                                                         */
/* -------------------------------------------------------------------------- */

/*
 * Create the raw-API listening socket on APP_SERVER_PORT and register the
 * application's accept callback. On failure the socket is released and the
 * rest of the stack keeps running.
 */
static void start_command_server(void)
{
    struct tcp_pcb *pcb = tcp_new();

    if (pcb == NULL) {
        return;
    }

    if (tcp_bind(pcb, IP_ADDR_ANY, APP_SERVER_PORT) == ERR_OK) {
        server_pcb = tcp_listen(pcb);
        tcp_accept(server_pcb, app_on_accept);
    } else {
        tcp_abort(pcb);        /* port busy or bind failed */
    }
}

/* -------------------------------------------------------------------------- */
/* Public entry point                                                         */
/* -------------------------------------------------------------------------- */

void lwip_run(void)
{
    sysarch_init();
    lwip_init();

    interface_init();
    start_command_server();

    /* Bare-metal main loop: service lwIP timers and the Ethernet driver. */
    for (;;) {
        sys_check_timeouts();
        EthIf_MainFunctionTx();
        EthIf_MainFunctionRx();
    }
}
