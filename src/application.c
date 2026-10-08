/*
 * Copyright 2017-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * application.c - bare-metal LED command server (the interesting part).
 *
 * A web browser connects to http://<board-ip>:8080/ and gets a small control
 * page with three buttons. Each button issues a request that this state
 * machine recognises and uses to toggle one of the board LEDs:
 *
 *     GET /        -> serve the HTML control page
 *     GET /green   -> toggle the GREEN LED
 *     GET /red     -> toggle the RED LED
 *     GET /blue    -> toggle the BLUE LED
 *     anything else-> 404 Not Found
 *
 * The flow is: lwIP accepts a connection (app_on_accept) and installs the
 * receive handler (app_on_recv). When the browser request arrives, route it to
 * the matching handler, drive the LED, and send an HTTP response.
 */

#include "application.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "Dio.h"
#include "lwip/pbuf.h"

/* -------------------------------------------------------------------------- */
/* LED state                                                                  */
/* -------------------------------------------------------------------------- */

/* Current on/off state of each LED so every request can toggle it. */
static bool led_red_on   = false;
static bool led_green_on = false;
static bool led_blue_on  = false;

/* -------------------------------------------------------------------------- */
/* HTML control page served on "GET /"                                        */
/* -------------------------------------------------------------------------- */

/*
 * Compact dark-theme page with three buttons. Each button issues a fetch() to
 * /green, /red or /blue, which this firmware handles below.
 */
static const char control_page[] =
    "<!DOCTYPE html><html data-theme=\"dark\"><head><meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
    "<title>S32K358 LED</title><style>"
    ":root{color-scheme:dark}"
    "body{margin:0;min-height:100vh;display:flex;flex-direction:column;"
    "align-items:center;justify-content:center;background:#0b0b0d;color:#e6e6e6;"
    "font-family:system-ui,Segoe UI,Roboto,Arial,sans-serif}"
    "h1{font-size:1.3rem;margin:0 0 24px}"
    ".b{display:flex;gap:16px;flex-wrap:wrap;justify-content:center}"
    "button{border:0;border-radius:8px;padding:14px 26px;font-size:1rem;"
    "font-weight:600;color:#fff;cursor:pointer;min-width:120px}"
    ".g{background:#2e9e4f}.r{background:#d13438}.u{background:#1f6feb}"
    "</style></head><body>"
    "<h1>Hello from FRDM-A-S32K358!</h1>"
    "<div class=\"b\">"
    "<button class=\"g\" onclick=\"fetch('/green')\">GREEN</button>"
    "<button class=\"r\" onclick=\"fetch('/red')\">RED</button>"
    "<button class=\"u\" onclick=\"fetch('/blue')\">BLUE</button>"
    "</div></body></html>";

/* -------------------------------------------------------------------------- */
/* Small HTTP helpers                                                         */
/* -------------------------------------------------------------------------- */

/* Send a complete, already-formatted response buffer and flush it. */
static void send_response(struct tcp_pcb *pcb, const char *data, u16_t len)
{
    (void)tcp_write(pcb, data, len, TCP_WRITE_FLAG_COPY);
    (void)tcp_output(pcb);
}

/* Send a short plain-text 200 OK reply (used after toggling an LED). */
static void send_text_ok(struct tcp_pcb *pcb, const char *body)
{
    char msg[128];
    int len = snprintf(msg, sizeof(msg),
                       "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/plain\r\n"
                       "Content-Length: %u\r\n"
                       "Connection: close\r\n\r\n%s",
                       (unsigned)strlen(body), body);
    send_response(pcb, msg, (u16_t)len);
}

/* Send the HTML control page. */
static void send_control_page(struct tcp_pcb *pcb)
{
    char hdr[96];
    int len = snprintf(hdr, sizeof(hdr),
                       "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html\r\n"
                       "Content-Length: %u\r\n"
                       "Connection: close\r\n\r\n",
                       (unsigned)(sizeof(control_page) - 1));
    (void)tcp_write(pcb, hdr, (u16_t)len, TCP_WRITE_FLAG_COPY);
    send_response(pcb, control_page, (u16_t)(sizeof(control_page) - 1));
}

/* Send an empty 404 Not Found. */
static void send_not_found(struct tcp_pcb *pcb)
{
    static const char resp[] =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Length: 0\r\n"
        "Connection: close\r\n\r\n";
    send_response(pcb, resp, sizeof(resp) - 1);
}

/* -------------------------------------------------------------------------- */
/* LED actions                                                                */
/* -------------------------------------------------------------------------- */

static void toggle_green(struct tcp_pcb *pcb)
{
    led_green_on = !led_green_on;
    Dio_WriteChannel(DioConf_DioChannel_GREEN,
                     led_green_on ? STD_HIGH : STD_LOW);
    send_text_ok(pcb, led_green_on ? "GREEN ON\n" : "GREEN OFF\n");
}

static void toggle_red(struct tcp_pcb *pcb)
{
    led_red_on = !led_red_on;
    Dio_WriteChannel(DioConf_DioChannel_RED,
                     led_red_on ? STD_HIGH : STD_LOW);
    send_text_ok(pcb, led_red_on ? "RED ON\n" : "RED OFF\n");
}

static void toggle_blue(struct tcp_pcb *pcb)
{
    led_blue_on = !led_blue_on;
    /* NOTE: adjust the channel below to the real BLUE LED channel. */
    Dio_WriteChannel(DioConf_DioChannel_BLUE,
                     led_blue_on ? STD_HIGH : STD_LOW);
    send_text_ok(pcb, led_blue_on ? "BLUE ON\n" : "BLUE OFF\n");
}

/* -------------------------------------------------------------------------- */
/* Request router (the state machine)                                         */
/* -------------------------------------------------------------------------- */

/* True if the request line starts with "GET <path> " or "GET <path>\r". */
static bool request_is(const char *req, u16_t len, const char *path)
{
    size_t plen = strlen(path);
    return (len >= plen) && (memcmp(req, path, plen) == 0);
}

/*
 * Inspect the first line of the HTTP request and dispatch to the matching
 * handler. This is the heart of the demo.
 */
static void route_request(struct tcp_pcb *pcb, const char *req, u16_t len)
{
    if (request_is(req, len, "GET /green")) {
        toggle_green(pcb);
    } else if (request_is(req, len, "GET /red")) {
        toggle_red(pcb);
    } else if (request_is(req, len, "GET /blue")) {
        toggle_blue(pcb);
    } else if (request_is(req, len, "GET / ")) {
        send_control_page(pcb);
    } else {
        send_not_found(pcb);
    }
}

/* -------------------------------------------------------------------------- */
/* lwIP raw-API callbacks                                                     */
/* -------------------------------------------------------------------------- */

/* Called when data arrives on an accepted connection. */
static err_t app_on_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p,
                         err_t err)
{
    LWIP_UNUSED_ARG(arg);

    if (p == NULL) {           /* remote side closed the connection */
        tcp_close(pcb);
        return ERR_OK;
    }
    if (err != ERR_OK) {       /* receive error: drop the buffer */
        pbuf_free(p);
        return err;
    }

    tcp_recved(pcb, p->tot_len);                 /* acknowledge the data */
    route_request(pcb, (const char *)p->payload, p->len);

    pbuf_free(p);
    tcp_close(pcb);            /* one request per connection, then close */
    return ERR_OK;
}

/* Called once per incoming connection; installs the receive handler. */
err_t app_on_accept(void *arg, struct tcp_pcb *new_pcb, err_t err)
{
    LWIP_UNUSED_ARG(arg);
    LWIP_UNUSED_ARG(err);
    tcp_recv(new_pcb, app_on_recv);
    return ERR_OK;
}
