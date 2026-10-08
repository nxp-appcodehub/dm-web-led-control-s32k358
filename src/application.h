/*
 * Copyright 2017-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * application.h - public interface of the bare-metal LED command server.
 *
 * This module contains the interesting part of the demo: a tiny HTTP-style
 * state machine that turns the board LEDs on/off depending on the URL that a
 * web browser requests. Everything related to configuring the lwIP stack
 * lives in lwip_general.c and stays out of the way.
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "lwip/err.h"
#include "lwip/tcp.h"

/* TCP port the LED command server listens on. */
#define APP_SERVER_PORT 8080

/*
 * TCP "accept" callback.
 *
 * lwIP calls this once per incoming connection. It is registered against the
 * listening PCB by lwip_general.c. Its only job is to attach the per-connection
 * receive handler (app_on_recv).
 */
err_t app_on_accept(void *arg, struct tcp_pcb *new_pcb, err_t err);

#endif /* APPLICATION_H */
