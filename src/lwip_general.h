/*
 * Copyright 2017-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LWIP_GENERAL_H
#define LWIP_GENERAL_H

/*
 * Initialise the lwIP stack and the LED command server, then enter the
 * bare-metal main loop. This function does not return.
 */
void lwip_run(void);

#endif /* LWIP_GENERAL_H */
