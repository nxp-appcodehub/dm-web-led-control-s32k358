/*
 *   Copyright 2025 - 2026 NXP
 *
 *   NXP Proprietary. This software is owned or controlled by NXP and may only be
 *   used strictly in accordance with the applicable license terms. By expressly
 *   accepting such terms or by downloading, installing, activating and/or otherwise
 *   using the software, you are agreeing that you have read, and that you agree to
 *   comply with and are bound by, such license terms. If you do not agree to be
 *   bound by the applicable license terms, then you may not retain, install,
 *   activate or otherwise use the software.
 *
 *   This file contains sample code only. It is not part of the production code deliverables.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "EthIf.h"
#include "EthIf_Cfg.h"
#include "eth_port.h"
/* Include drivers APIs and configurations. */
/* Include upper layers APIs and callbacks. */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAX_CTRLIDX_SUPPORTED           (1u)
#define MAX_PORTIDX_SUPPORTED           (1u)
#define MAX_TRCVIDX_SUPPORTED           (1u)
#define MAX_TXCONFIRMATIONS             (1u)
#define MAX_CTRL_MODE_INDICATIONS       (0u)
#define MAX_TRCV_LINK_STATE_CBKS        (0u)
#define MAX_SWITCHES                    (0u)
#define MAX_SWITCH0_PORTS               (0u)

/* Frame type filtering on reception. */
#define MAX_FRAME_TYPE_FILTERS          (4u)
#define MAX_ALLWAYS_INDICATIONS         (0u)
#define FR_ETH_ARP                      (0x0806u)
#define FR_ETH_IP4						(0x0800u)
#define FR_ETH_IP6						(0x86DDu)
#define FR_ETH_LLDP						(0x86CCu)
#define MAX_FILTERED_IP_INDICATIONS     (1u)

/*******************************************************************************
 * Variables
 ******************************************************************************/

EthIf_Cfg_Ctrl_Func_Table_t             arEthIfCfgFuncMap[MAX_CTRLIDX_SUPPORTED] =
{
    /* Example controller index 1 as GMAC. */
    {
        .pfEth_ProvideTxBuffer            = Eth_43_GMAC_ProvideTxBuffer,
        .pfEthIf_EnableEgressTimeStamp    = NULL_PTR,
        .pfEthIf_SetSwitchMgmtInfo        = NULL_PTR,
        .pfEthIf_SwitchEnableTimeStamping = NULL_PTR,
        .pfEth_Transmit                   = Eth_43_GMAC_Transmit,
        .pfEthSwt_GetTxMgmtObject         = NULL_PTR,
        .pfEthSwt_GetRxMgmtObject         = NULL_PTR,
        .pfEth_GetPhysAddr                = NULL_PTR,
        .pfEthIf_GetEgressTimeStamp       = NULL_PTR,
        .pfEthIf_GetIngressTimeStamp      = NULL_PTR,
        .pfEthIf_UpdatePhysAddrFilter     = Eth_43_GMAC_UpdatePhysAddrFilter,
        .pfEth_TxConfirmation             = Eth_43_GMAC_TxConfirmation,
        .pfEth_Receive                    = Eth_43_GMAC_Receive,
		.pfEthIf_SetControllerMode        = Eth_43_GMAC_SetControllerMode,
        .pfEthIf_GetCurrentTime           = NULL_PTR,
        .pfEthIf_GetCurrentTimeTuple      = NULL_PTR,
        .pfEthIf_SetPhcTime               = NULL_PTR,
        .pfEthIf_GetPhcTime               = NULL_PTR,
        .pfEthIf_SetPhcCorrection         = NULL_PTR,
        .pfEthIf_SetPpsSignalMode         = NULL_PTR,
        .u8MaxReceiveFifos                = ETH_43_GMAC_MAX_RXFIFO_SUPPORTED /* Controller specific FIFO count define. */
    }
};

/* Example of EthIf port indexes mapped to controllers and their ports. */
EthIf_Ctrl_Port_t                       arEthIfToCtrlPortMap[MAX_PORTIDX_SUPPORTED] =
{
    /* Example of controller 0, port 0 for EthIf index 0 on transceiver 0. */
    {
        .u8CtrlIdx     = 0,
        .u8CtrlPortIdx = 0,
        .u8TrcvIdx     = 0,
    }
};

/* Example of function registered for all ethertypes. */
static ethif_rx_indication_cbk_t        apfUL_AllwaysRxIndications[MAX_ALLWAYS_INDICATIONS] =
{
    NULL_PTR /* Function must be defined by user. */
};

/* Example of function registered for FR_ETH_IP. */
static ethif_rx_indication_cbk_t        apfCustRxIndications[MAX_FILTERED_IP_INDICATIONS] =
{
	Tcpip_RxIndication /* Function must be defined by user. */
};

/* Example of ethertype filtering. */
static EthIf_Cbk_Rx_Indications_t       arEthIfRxIndRegister[MAX_FRAME_TYPE_FILTERS] =
{
    {
        .u16FrameType        = FR_ETH_ARP,
        .u8CbksCount         = MAX_FILTERED_IP_INDICATIONS,
        .ppfUL_RxIndications = apfCustRxIndications
    },
	{
	    .u16FrameType        = FR_ETH_IP4,
	    .u8CbksCount         = MAX_FILTERED_IP_INDICATIONS,
	    .ppfUL_RxIndications = apfCustRxIndications
	},
	{
		.u16FrameType        = FR_ETH_IP6,
		.u8CbksCount         = MAX_FILTERED_IP_INDICATIONS,
		.ppfUL_RxIndications = apfCustRxIndications
	},
	{
		.u16FrameType        = FR_ETH_LLDP,
		.u8CbksCount         = MAX_FILTERED_IP_INDICATIONS,
		.ppfUL_RxIndications = apfCustRxIndications
	}
};

static ethif_tx_confirmation_cbk_t      apfUL_TxConfirmations[MAX_TXCONFIRMATIONS] =
{
	Tcpip_TxConfirmation /* Arrays must have at least one element. */
};

static ethif_ctrl_mode_indication_cbk_t apfUL_CtrlModeIndications[MAX_CTRL_MODE_INDICATIONS] =
{
    NULL_PTR /* Arrays must have at least one element. */
};

static ethif_trcv_link_state_chg_cbk_t  apfUL_TrcvLinkStateChg[MAX_TRCV_LINK_STATE_CBKS] =
{
    NULL_PTR /* Arrays must have at least one element. */
};

EthIf_Cfg_Cbk_Func_Table_t              rEthIfCfgCbkFuncMap =
{
    .prRxIndicationsCfg            = arEthIfRxIndRegister,
    .u8RxCfgsCount                 = MAX_FRAME_TYPE_FILTERS,
    .rTxConfirmationsCfg           =
    {
        .u8CbksCount           = MAX_TXCONFIRMATIONS,
        .ppfUL_TxConfirmations = apfUL_TxConfirmations
    },
    .rCtrlModeIndicationsCfg       =
    {
        .u8CbksCount               = MAX_CTRL_MODE_INDICATIONS,
        .ppfUL_CtrlModeIndications = apfUL_CtrlModeIndications
    },
    .rTrcvLinkStateChgCfg          =
    {
        .u8CbksCount            = MAX_TRCV_LINK_STATE_CBKS,
        .ppfUL_TrcvLinkStateChg = apfUL_TrcvLinkStateChg
    },
    .ppfUL_RxIndicationsAlways     = apfUL_AllwaysRxIndications,
    .u8AlwaysRxCount               = MAX_ALLWAYS_INDICATIONS
};

#if (0 < MAX_SWITCHES)
static uint8_t                          au8PortsInSwitches[MAX_SWITCHES] =
{
    MAX_SWITCH0_PORTS
};
#endif /* (0 < MAX_SWITCHES) */

EthIf_Config_t                          rEthIfCompConfig =
{
    .u8Controllers  = MAX_CTRLIDX_SUPPORTED,
    .u8Ports        = MAX_PORTIDX_SUPPORTED,
    .u8Transceivers = MAX_TRCVIDX_SUPPORTED,
    .u8Switches     = MAX_SWITCHES,
#if (0 < MAX_SWITCHES)
    .pu8SwitchPorts  = au8PortsInSwitches,
#else
    .pu8SwitchPorts  = NULL_PTR,
#endif /* (0 < MAX_SWITCHES) */
    .arPortMap      = arEthIfToCtrlPortMap
};

/* Last link status for change detection. */
EthTrcv_LinkStateType                   seLastLinkState[MAX_PORTIDX_SUPPORTED];

/* Transceivers modes array. */
EthTrcv_ModeType                        aeTrcvsModes[MAX_TRCVIDX_SUPPORTED];

/* Ports modes array for each switch. */
#if (0 < MAX_SWITCHES)
EthTrcv_ModeType                        aeSwitch0PortsModes[MAX_SWITCH0_PORTS];

EthTrcv_ModeType                        *apeSwitchPortsModes[MAX_SWITCHES] =
{
    aeSwitch0PortsModes
};
#else
EthTrcv_ModeType                        *apeSwitchPortsModes[1] =
{
    NULL_PTR /* Arrays must have at least one element. */
};
#endif /* (0 < MAX_SWITCHES) */

/*******************************************************************************
 * Data Types
 ******************************************************************************/
/* None */

/*******************************************************************************
 * Externs
 ******************************************************************************/
/* None */

/*******************************************************************************
 * API
 ******************************************************************************/
/* None */

/*******************************************************************************
 * EOF
 ******************************************************************************/
