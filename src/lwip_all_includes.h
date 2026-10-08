/*
 * Copyright 2017-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LWIP_ALL_INCLUDES_H
#define LWIP_ALL_INCLUDES_H

/* C runtime */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Board I/O (LED control) */
#include "Dio.h"

/* lwIP core */
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "lwip/netif.h"
#include "lwip/tcp.h"

/* lwIP network interface glue */
#include "lwip/etharp.h"
#include "netif/ethernet.h"

/* Timer sources needed when running without an OS (NO_SYS=1) */
#include "lwip/ip4_frag.h"
#include "lwip/igmp.h"

/* Port-dependent configuration and Ethernet driver glue */
#include "netifcfg.h"
#include "lwipcfg.h"
#include "eth_port.h"

#endif /* LWIP_ALL_INCLUDES_H */
