/*==================================================================================================
*
*   Copyright 2026 NXP
*
*   NXP Proprietary. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
*
==================================================================================================*/

/*
 * Bare-metal lwIP LED command server - entry point.
 *
 * Flow:
 *     main()        -> device_init() : MCU clocks, pins, Ethernet MAC
 *                   -> lwip_run()    : lwIP bring-up + main loop (lwip_general.c)
 *
 * The actual request handling that drives the LEDs lives in application.c.
 */

#include "device.h"
#include "lwip_general.h"

int main(void)
{
    /* Bring up the microcontroller (clocks, pins, Ethernet). */
    device_init();

    /* Start lwIP and the LED command server. Does not return. */
    lwip_run();

    return 0;
}
