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

#include "S32K358.h"
#include "device.h"
#include "Mcal.h"
#include "Mcu.h"
#include "Port.h"
#include "EthIf.h"
#include "OsIf.h"
#include "Platform.h"
#include "Eth_43_GMAC.h"

#ifndef USING_OS_FREERTOS
#include "Gpt.h"
#endif

void device_init(void)
{
    /* Set RMII configuration for EMAC in DCM module */
	IP_DCM_GPR->DCMRWF1 = (IP_DCM_GPR->DCMRWF1 & ~DCM_GPR_DCMRWF1_MAC_CONF_SEL_MASK) | DCM_GPR_DCMRWF1_MAC_CONF_SEL(2U);

    /* Initialize Os Interface */
    OsIf_Init(NULL_PTR);

    /* Initialize all pins using the Port driver */
    Port_Init(NULL_PTR);

    /* Setup Clocks */
    /* Initialize Mcu module */
    Mcu_Init(NULL_PTR);

    /* Initialize Mcu clock */
    Mcu_InitClock(McuClockSettingConfig_0);

    while (Mcu_GetPllStatus() != MCU_PLL_LOCKED){};

    /* Use PLL clock */
    Mcu_DistributePllClock();

    Mcu_SetMode(McuModeSettingConf_0);

    /* Initialize Platform driver */
    Platform_Init(NULL_PTR);

#ifndef USING_OS_FREERTOS

    /* Initialize PIT driver and start the timer */
    Gpt_Init(&Gpt_Config);
    /* Start the Gpt timer */
    Gpt_StartTimer(GptConf_GptChannelConfiguration_GptChannelConfiguration_0, 40000U);
    /* Enable the Gpt notification*/
    Gpt_EnableNotification(GptConf_GptChannelConfiguration_GptChannelConfiguration_0);

    OsIf_SetTimerFrequency(160000000U,  OSIF_USE_SYSTEM_TIMER);

#endif /* USING_OS_FREERTOS */

    /* Initialize and enable the GMAC module */
    Eth_43_GMAC_Init(NULL_PTR);
}
