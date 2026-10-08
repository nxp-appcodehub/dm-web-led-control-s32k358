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
 * Guard Begin
 ******************************************************************************/

#ifndef ETHIF_CFG_H
#define ETHIF_CFG_H

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "BasicTypes.h"
#include "Eth_GeneralTypes.h"
#include "ComStack_Types.h"
#include "Eth_43_GMAC.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief           Provides access to a transmit buffer of the specified
 *                  Ethernet controller.
 *
 * @param[in]       u8CtrlIdx Index of the Ethernet controller within
 *                  the context of the Ethernet Driver.
 * @param[in]       eFrameType       Ethernet Frame Type (EtherType).
 * @param[in]       u8Priority       Priority value which shall be used for
 *                                   the 3-bit PCP field of the VLAN tag.
 * @param[out]      peBufIdxPtr      Index to the granted buffer resource.
 *                                   To be used for subsequent requests.
 * @param[out]      pu8BufPtr        Pointer to the granted buffer.
 * @param[inout]    pu16LenBytePtr   in: desired length in bytes
 *                                   out: granted length in bytes
 *
 * @return          BufReq_Return-Type
 * @retval          BUFREQ_OK: Success.
 * @retval          BUFREQ_E_NOT_OK: Development error detected.
 * @retval          BUFREQ_E_BUSY: All buffers in use.
 * @retval          BUFREQ_E_OVFL: Requested buffer too large.
*/
typedef BufReq_ReturnType (*ethif_provide_tx_buffer_t)(uint8 u8CtrlIdx,
                                                       uint8 u8Priority,
                                                       Eth_BufIdxType *peBufIdxPtr,
                                                       uint8 **pu8BufPtr,
                                                       uint16 *pu16LenBytePtr);

/*!
 * @brief           Enables egress timestamp.
 *
 * @param[in]       u8CtrlIdx Index of the Ethernet controller within
 *                  the context of the Ethernet Driver.
 * @param[in]       eBufIdx Index of the message buffer, where Application
 *                  expects egress time stamping.
*/
typedef void (*ethif_enable_egress_time_stamp_t)(uint8 u8CtrlIdx,
                                                 Eth_BufIdxType eBufIdx);

/*!
 * @brief           Provides additional management information along to
 *                  an Ethernet frame that requires special treatment within
 *                  the Switch.
 *
 * @details         It has to be called between EthIf_ProvideTxBuffer()
 *                  and EthIf_Transmit() of the related frame.
 *
 * @param[in]       u8CtrlIdx Index of an Ethernet Interface controller.
 * @param[in]       eBufIdx Ethernet Tx Buffer index.
 * @param[in]       psMgmtInfoPtr Pointer to the management information.
 *
 * @return          Std_ReturnType
 * @retval          E_OK: Management infos successfully set.
 * @retval          E_NOT_OK: Setting of management infos failed.
*/
typedef Std_ReturnType (*ethif_set_switch_mgmt_info_t)(uint8 u8CtrlIdx,
                                                       Eth_BufIdxType eBufIdx,
                                                       EthSwt_MgmtInfoType *psMgmtInfoPtr);

/*!
 * @brief           Activates egress timestamping on a dedicated message object,
 *                  addressed by CtrlIdx and BufIdx.
 *
 * @param[in]       u8CtrlIdx Index of an Ethernet Interface controller.
 * @param[in]       eBufIdx Ethernet Tx Buffer index.
 * @param[out]      psMgmtInfo Management information.
 *
 * @return          Std_ReturnType
 * @retval          E_OK: Timestamping on egress successfully enabled.
 * @retval          E_NOT_OK: Enabling of timestamping on egress failed.
*/
typedef Std_ReturnType (*ethif_switch_enable_time_stamping_t)(uint8 u8CtrlIdx,
                                                              Eth_BufIdxType eBufIdx,
                                                              EthSwt_MgmtInfoType *psMgmtInfo);

/*!
 * @brief           Triggers transmission of a previously filled transmit
 *                  buffer.
 *
 * @param[in]       u8CtrlIdx Index of an Ethernet Interface controller.
 * @param[in]       eBufIdx Index of the buffer resource.
 * @param[in]       eFrameType Ethernet Frame Type (EtherType).
 * @param[in]       bTxConfirmation Activates transmission confirmation.
 * @param[in]       u16LenByte Data length in byte.
 * @param[in]       cpu8PhysAddrPtr Physical target address (MAC address)
 *                  in network byte order.
 *
 * @return          Std_ReturnType
 * @retval          E_OK: Transmission success.
 * @retval          E_NOT_OK: Transmission failed.
*/
typedef Std_ReturnType (*ethif_transmit_t)(uint8 u8CtrlIdx,
                                           Eth_BufIdxType eBufIdx,
                                           Eth_FrameType eFrameType,
                                           boolean bTxConfirmation,
                                           uint16 u16LenByte,
                                           const uint8 *cpu8PhysAddrPtr);

/*!
 * @brief           Request the MgmtObject of the (in this context) unique
 *                  BufIdx.
 *
 * @param[in]       u8CtrlIdx Index of an Ethernet Interface controller.
 * @param[in]       eBufIdx Ethernet Rx Buffer index.
 * @param[out]      psMgmtObjectPtr Pointer to the management object.
 *
 * @return          Std_ReturnType
 * @retval          E_OK: Success.
 * @retval          E_NOT_OK: Management object could not be obtained.
*/
typedef Std_ReturnType (*ethif_get_tx_mgmt_object_t)(uint8 u8CtrlIdx,
                                                     Eth_BufIdxType eBufIdx,
                                                     EthSwt_MgmtObjectType **psMgmtObjectPtr);

/*!
 * @brief           Request the MgmtObject of the (in this context) unique
 *                  DataPtr.
 *
 * @param[in]       u8CtrlIdx Index of an Ethernet Interface controller.
 * @param[in]       peDataPtr Ethernet data pointer.
 * @param[out]      psMgmtObjectPtr Pointer to the management object.
 *
 * @return          Std_ReturnType
 * @retval          E_OK: Success.
 * @retval          E_NOT_OK: Management object could not be obtained.
*/
typedef Std_ReturnType (*ethif_get_rx_mgmt_object_t)(uint8 u8CtrlIdx,
                                                     Eth_DataType *peDataPtr,
                                                     EthSwt_MgmtObjectType **psMgmtObjectPtr);

/*!
 * @brief           Obtains the physical source address used by the indexed
 *                  controller.
 *
 *
 * @param[in]       u8CtrlIdx Index of an Ethernet Interface controller.
 * @param[out]      pu8PhysAddrPtr Physical source address (MAC address)
 *                  in network byte order.
*/
typedef void (*ethif_get_phys_addr_t)(uint8 u8CtrlIdx,
                                      uint8 *pu8PhysAddrPtr);

/*!
 * @brief           Get egress timestamp.
 *
 * @param[in]       u8CtrlIdx Index of the Ethernet controller within
 *                  the context of the Ethernet Driver.
 * @param[in]       eBufIdx Index of the message buffer, where the Upper Layer
 *                  expects egress time stamping.
 * @param[inout]    psTimeQual Quality of HW time stamp, e.g. based on current
 *                  drift.
 * @param[out]      psTimeStamp Current time stamp.
 *
 * @return          Std_ReturnType
 * @retval          E_OK: Success.
 * @retval          E_NOT_OK: Failed to read time stamp.
*/
typedef Std_ReturnType (*ethif_get_egress_time_stamp_t)(uint8 u8CtrlIdx,
                                                        Eth_BufIdxType eBufIdx,
                                                        Eth_TimeStampQualType *psTimeQual,
                                                        Eth_TimeStampType *psTimeStamp);

/*!
 * @brief           Get ingress timestamp.
 *
 * @param[in]       u8CtrlIdx Index of the Ethernet controller within
 *                  the context of the Ethernet Driver.
 * @param[in]       pEthData Pointer to the message buffer, where Application
 *                  expects ingress time stamping.
 * @param[out]      psTimeQual Quality of HW time stamp, e.g. based on current
 *                  drift.
 * @param[out]      psTimeStamp Current time stamp.
 *
 * @return          Std_ReturnType
 * @retval          E_OK: Success.
 * @retval          E_NOT_OK: Failed to read time stamp.
*/
typedef Std_ReturnType (*ethif_get_ingress_time_stamp_t)(uint8 u8CtrlIdx,
                                                         const Eth_DataType *pEthData,
                                                         Eth_TimeStampQualType *psTimeQual,
                                                         Eth_TimeStampType *psTimeStamp);

/*!
 * @brief           Update the physical source address to/from the indexed
 *                  controller filter.
 *
 * @param[in]       u8CtrlIdx Index of the Ethernet controller within
 *                  the context of the Ethernet Driver.
 * @param[in]       cpu8PhysAddrPtr Pointer to memory containing the physical
 *                  destination address (MAC address) in network byte order.
 *                  This is the multicast destination address of the layer 2
 *                  Ethernet packet.
 * @param[in]       eAction Add or remove the address from the Ethernet
 *                  controllers filter.
 *
 * @return          Std_ReturnType
 * @retval          E_OK: Filter was successfully changed.
 * @retval          E_NOT_OK: Filter could not be changed.
*/
typedef Std_ReturnType (*ethif_update_phys_addr_filter_t)(uint8 u8CtrlIdx,
                                                          const uint8 *cpu8PhysAddrPtr,
                                                          Eth_FilterActionType eAction);

/*!
 * @brief           Triggers frame transmission confirmations.
 *
 * @param[in]       u8CtrlIdx Index of the controller which shall be checked
 *                  whether any frame transmission has finished. The index
 *                  is valid within the context of the Ethernet Driver only.
*/
typedef void (*ethif_tx_confirmation_t)(uint8 u8CtrlIdx);

/*!
 * @brief           Triggers frames reception notifications.
 *
 * @param[in]       u8CtrlIdx Index of the controller which shall be checked
 *                  whether any new frames were received. The index is valid
 *                  within the context of the Ethernet Driver only.
 * @param[in]       u8FifoIdx Index of Rx Fifo where frames were received.
 * @param[out]      peRxStatusPtr Informs the caller whether a frame was
 *                  received (@c ETH_RECEIVED or @c ETH_NOT_RECEIVED) and
 *                  whether more frames are available in the queue
 *                  (@c ETH_RECEIVED or @c ETH_RECEIVED_MORE_DATA_AVAILABLE).
*/
typedef void (*ethif_receive_t)(uint8 u8CtrlIdx,
                                uint8 u8FifoIdx,
                                Eth_RxStatusType *peRxStatusPtr);

/*!
 * @brief           This function provides time value out of the HW registers according
 *                  to the capability of the HW.
 * @details
 * @param[in]       u8CtrlIdx Index of the addresses ETH controller.
 * @param[out]      timeQualPtr Quality of HW time stamp, e.g. based on current drift.
 * @param[out]      timeStampPtr Current timestamp.
 * @return          Std_ReturnType
 * @retval          E_OK: Successful.
 * @retval          E_NOT_OK: Failed.
*/
typedef Std_ReturnType (*ethif_get_current_time_t)(uint8 u8CtrlIdx,
                                                   Eth_TimeStampQualType* timeQualPtr,
                                                   Eth_TimeStampType* timeStampPtr);

/*!
 * @brief           This function reads the current time of the timestamp clock and the current
 *                  time of the PHC in an atomic operation.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of the Ethernet
                              Interface which owns the clock unit.
 * @param[in]       u8ClkUnitIdx Index of the Clock Unit within the context of the Ethernet Interface
                                 to provide the time tuple.
 * @param[out]      currentTimeTuplePtr Current time tuple with the (1) value of the free-running
 *                                      clock used for timestamping, (2) alue of the adjustable PHC.
 * @return          Std_ReturnType
 * @retval          E_OK: Current time successfully retrieved.
 * @retval          E_NOT_OK: Current time could not be retrieved.
*/
typedef Std_ReturnType (*ethif_get_current_time_tuple_t)(uint8 u8CtrlIdx,
                                                         uint8 u8ClkUnitIdx,
                                                         TimeTupleType* currentTimeTuplePtr);

/*!
 * @brief           This function sets the absolute time of the PHC.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of the Ethernet
                              Interface which owns the clock unit.
 * @param[in]       u8ClkUnitIdx Index of the Clock Unit within the context of the Ethernet Interface
                                 to provide the time tuple.
 * @param[in]       timeStampPtr Time value to which the PHC shall be set.
 * @return          Std_ReturnType
 * @retval          E_OK: PHC successfully set.
 * @retval          E_NOT_OK: PHC could not be set.
*/
typedef Std_ReturnType (*ethif_set_phc_time_t)(uint8 u8CtrlIdx,
                                               uint8 u8ClkUnitIdx,
                                               const TimeStampType* timeStampPtr);

/*!
 * @brief           This function returns the current time value out of the HW registers of the PHC.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of the Ethernet
                              Interface which owns the clock unit.
 * @param[in]       u8ClkUnitIdx Index of the Clock Unit within the context of the Ethernet Interface
                                 to provide the time tuple.
 * @param[in]       timeQualPtr Quality of HW time stamp, e.g. based on current drift.
 * @param[in]       timeStampPtr Current timestamp.
 * @return          Std_ReturnType
 * @retval          E_OK: PHC value successfully retrieved.
 * @retval          E_NOT_OK: PHC value could not be retrieved.
*/
typedef Std_ReturnType (*ethif_get_phc_time_t)(uint8 u8CtrlIdx,
                                               uint8 u8ClkUnitIdx,
                                               TimeStampQualType* timeQualPtr,
                                               TimeStampType* timeStampPtr);

/*!
 * @brief           This function sets PHC parameters to adapt rate and offset of the PHC.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of the Ethernet
                              Interface which owns the clock unit.
 * @param[in]       u8ClkUnitIdx Index of the Clock Unit within the context of the Ethernet Interface
                                 to provide the time tuple.
 * @param[in]       s32RateDeviation Rate deviation (resolution: 2-41), by which the PHC is requested
 * @param[in]       s32Offset Time offset, by which the PHC is requested to be updated.
 * @return          Std_ReturnType
 * @retval          E_OK: PHC successfully set.
 * @retval          E_NOT_OK: PHC could not be set.
*/
typedef Std_ReturnType (*ethif_set_phc_correction_t)(uint8 u8CtrlIdx,
                                                     uint8 u8ClkUnitIdx,
                                                     sint32 s32RateDeviation,
                                                     sint32 s32Offset);

/*!
 * @brief           This function enables/disables the generation of a PPS signal.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of the Ethernet
                              Interface which owns the clock unit.
 * @param[in]       u8ClkUnitIdx Index of the Clock Unit within the context of the Ethernet Interface
                                 to provide the time tuple.
 * @param[in]       bSignalMode TRUE: PPS signal generation is enabled,
 *                              FALSE: PPS signal generation is disabled.
 * @return          Std_ReturnType
 * @retval          E_OK: PPS signal generation successfully enabled/disabled.
 * @retval          E_NOT_OK: Failed to enable/disable PPS signal generation.
*/
typedef Std_ReturnType (*ethif_set_pps_signal_mode_t)(uint8 u8CtrlIdx,
                                                      uint8 u8ClkUnitIdx,
                                                      boolean bSignalMode);

/*!
 * @brief           Enables / disables the indexed controller.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of the Ethernet
                              Interface.
 * @param[in]       enCtrlMode ETH_MODE_DOWN: disable the controller
 * 							   ETH_MODE_ACTIVE: enable the controller
 * 							   ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST: enable the controller and request a wake-up on the network.
 * 							   ETH_MODE_TX_OFFLINE: disable transmission handling in Eth If. Please note, the according Ethernet controller is not affected.
 * @return          Std_ReturnType
 * @retval          E_OK: Success.
 * @retval          E_NOT_OK: CtrlMode could not be changed.
*/
typedef Std_ReturnType (*ethif_set_controller_mode_t)(uint8 u8CtrlIdx,
                                                     Eth_ModeType enCtrlMode);

/*!
 * @brief           Controller interface function table.
 */
typedef struct
{
    /*! Access to a transmit buffer of the controller. */
    ethif_provide_tx_buffer_t           pfEth_ProvideTxBuffer; /* 8 bytes */
    /*! Enables egress timestamp on the controller. */
    ethif_enable_egress_time_stamp_t    pfEthIf_EnableEgressTimeStamp; /* 8 bytes */
    /*! Provides additional management information. */
    ethif_set_switch_mgmt_info_t        pfEthIf_SetSwitchMgmtInfo; /* 8 bytes */
    /*! For activation of egress timestampingon the controller. */
    ethif_switch_enable_time_stamping_t pfEthIf_SwitchEnableTimeStamping; /* 8 bytes */
    /*! Triggers transmission on the controller. */
    ethif_transmit_t                    pfEth_Transmit; /* 8 bytes */
    /*! Request the MgmtObject from the controller. */
    ethif_get_tx_mgmt_object_t          pfEthSwt_GetTxMgmtObject; /* 8 bytes */
    /*! Request the MgmtObject from the controller. */
    ethif_get_rx_mgmt_object_t          pfEthSwt_GetRxMgmtObject; /* 8 bytes */
    /*! Obtains the physical source address of the controller. */
    ethif_get_phys_addr_t               pfEth_GetPhysAddr; /* 8 bytes */
    /*! Get egress timestamp from the controller. */
    ethif_get_egress_time_stamp_t       pfEthIf_GetEgressTimeStamp; /* 8 bytes */
    /*! Get ingress timestamp from the controller. */
    ethif_get_ingress_time_stamp_t      pfEthIf_GetIngressTimeStamp; /* 8 bytes */
    /*! Updates physical address filter in the controller. */
    ethif_update_phys_addr_filter_t     pfEthIf_UpdatePhysAddrFilter; /* 8 bytes */
    /*! Triggers frame transmission confirmations. */
    ethif_tx_confirmation_t             pfEth_TxConfirmation; /* 8 bytes */
    /*! Triggers frames reception notifications. */
    ethif_receive_t                     pfEth_Receive; /* 8 bytes */
    /*! Enables / disables the indexed controller. */
    ethif_set_controller_mode_t         pfEthIf_SetControllerMode; /* 8 bytes */
    /*! Get a time value out of the HW registers. */
    ethif_get_current_time_t            pfEthIf_GetCurrentTime; /* 8 bytes */
    /*! Get the current time of the timestamp clock and the current time of the PHC. */
    ethif_get_current_time_tuple_t      pfEthIf_GetCurrentTimeTuple; /* 8 bytes */
    /*! Set the absolute time of the PHC. */
    ethif_set_phc_time_t                pfEthIf_SetPhcTime; /* 8 bytes */
    /*! Get the current time value out of the HW registers of the PHC. */
    ethif_get_phc_time_t                pfEthIf_GetPhcTime; /* 8 bytes */
    /*! Set PHC parameters to adapt rate and offset of the PHC. */
    ethif_set_phc_correction_t          pfEthIf_SetPhcCorrection; /* 8 bytes */
    /*! Enables/disables the generation of a PPS signal. */
    ethif_set_pps_signal_mode_t         pfEthIf_SetPpsSignalMode; /* 8 bytes */
    /*! Controller specific receive FIFO count. */
    uint8                               u8MaxReceiveFifos; /* 1 byte */
    /* 7 bytes padding */
    /* 160 bytes total */
} EthIf_Cfg_Ctrl_Func_Table_t;

/*!
 * @brief           This function handles the received Ethernet frame.
 * @details         Function should parse the received frame and pass
 *                  the gathered information to the appropriate upper layer
 *                  module.
 * @note            The passed data buffer is no longer valid after the function
 *                  is exited.
 *
 * @param[in]       u8CtrlIdx Index of the controller which received the frame.
 * @param[in]       u16FrameType The received frame Ethertype
 *                  (from the frame header).
 * @param[in]       bIsBroadcast   The value TRUE indicates that the received
 *                  frame was sent to broadcast address (ff-ff-ff-ff-ff-ff).
 * @param[in]       cu8PhysAddrPtr Pointer to received frame source MAC address
 *                  (6 bytes).
 * @param[in]       cu8DataPtr Data buffer containing the received Ethernet
 *                  frame payload.
 * @param[in]       u16LenByte Length of the data in the buffer DataPtr.
*/
typedef void (*ethif_rx_indication_cbk_t)(uint8 u8CtrlIdx,
                                          Eth_FrameType u16FrameType,
                                          boolean bIsBroadcast,
                                          const uint8 *cu8PhysAddrPtr,
                                          const Eth_DataType *cu8DataPtr,
                                          uint16 u16LenByte);

/*!
 * @brief           Multiple Rx indications register filter structure.
 */
typedef struct
{
    /*! Registered RxIndications to upper layers. */
    ethif_rx_indication_cbk_t           *ppfUL_RxIndications; /* 8 bytes */
    /*! EtherType for which RxIndications will be called. */
    Eth_FrameType                       u16FrameType; /* 2 bytes */
    /*! Count of registered callbacks. */
    uint8                               u8CbksCount; /* 1 byte */
    /* 5 bytes padding */
    /* 16 bytes total */
} EthIf_Cbk_Rx_Indications_t;

/*!
 * @brief           This function confirms that transmission of an Ethernet
 *                  frame was finished.
 * @details         Function should notify the appropriate upper layer module 
 *                  that the data transmission was successfully finished.
 *
 * @param[in]       u8CtrlIdx Index of the controller which transmitted
 *                  the frame.
 * @param[in]       u32BufferIndex Index of the transmitted data buffer.
 * @param[in]       eResult Result.
*/
typedef void (*ethif_tx_confirmation_cbk_t)(uint8 u8CtrlIdx,
                                            Eth_BufIdxType u32BufferIndex,
                                            Std_ReturnType eResult);

/*!
 * @brief           Multiple tx confirmations register structure.
 */
typedef struct
{
    /*! Registered TxConfiramtions to upper layers. */
    ethif_tx_confirmation_cbk_t         *ppfUL_TxConfirmations; /* 8 bytes */
    /*! Count of registered callbacks. */
    uint8                               u8CbksCount; /* 1 byte */
    /* 7 bytes padding */
    /* 16 bytes total */
} EthIf_Cbk_Tx_Confirmations_t;

/*!
 * @brief           This function indicate that driver mode has been changed.
 * @details         Called asynchronously when mode has been read out.
 *                  Triggered by previous Eth_SetControllerMode call.
 *                  Can directly be called within the trigger functions.
 *
 * @param[in]       u8CtrlIdx Index of the controller which mode has been
 *                  changed.
 * @param[in]       eCtrlMode New mode of correspond Eth driver.
*/
typedef void (*ethif_ctrl_mode_indication_cbk_t)(uint8 u8CtrlIdx,
                                                 Eth_ModeType eCtrlMode);

/*!
 * @brief           Multiple controller mode indicatios register structure.
 */
typedef struct
{
    /*! Registered driver mode indications to upper layers. */
    ethif_ctrl_mode_indication_cbk_t    *ppfUL_CtrlModeIndications; /* 8 bytes */
    /*! Count of registered callbacks. */
    uint8                               u8CbksCount; /* 1 byte */
    /* 7 bytes padding */
    /* 16 bytes total */
} EthIf_Cbk_Ctrl_Mode_Indications_t;

/*!
 * @brief           This function indicates that a transceiver's link state was
 *                  changed.
 * @details         Function should notify the appropriate upper layer module
 *                  that the link state of transceiver module has been changed.
 *
 * @param[in]       u8CtrlIdx Index of the controller which link state has
 *                  been changed.
 * @param[in]       eTrcvLinkState Notified transceiver link state.
*/
typedef void (*ethif_trcv_link_state_chg_cbk_t)(uint8 u8CtrlIdx,
                                                EthTrcv_LinkStateType eTrcvLinkState);

/*!
 * @brief           Multiple link state change register structure.
 */
typedef struct
{
    /*! Registered link state change callbacks to upper layers. */
    ethif_trcv_link_state_chg_cbk_t     *ppfUL_TrcvLinkStateChg; /* 8 bytes */
    /*! Count of registered callbacks. */
    uint8                               u8CbksCount; /* 1 byte */
    /* 7 bytes padding */
    /* 16 bytes total */
} EthIf_Cbk_Trcv_Link_State_Chg_t;

/*!
 * @brief           Callbacks interface configuration structure.
 */
typedef struct
{
    EthIf_Cbk_Tx_Confirmations_t        rTxConfirmationsCfg; /* 16 bytes */
    EthIf_Cbk_Ctrl_Mode_Indications_t   rCtrlModeIndicationsCfg; /* 16 bytes */
    EthIf_Cbk_Trcv_Link_State_Chg_t     rTrcvLinkStateChgCfg; /* 16 bytes */
    EthIf_Cbk_Rx_Indications_t          *prRxIndicationsCfg; /* 8 bytes */
    /*! Registered RxIndications to upper layers called
        for all ethernet frame types. */
    ethif_rx_indication_cbk_t           *ppfUL_RxIndicationsAlways; /* 8 bytes */
    /*! Count of configured RxIndications filters. */
    uint8                               u8RxCfgsCount; /* 1 byte */
    uint8                               u8AlwaysRxCount; /* 1 byte */
    /* 6 bytes padding */
    /* 72 bytes total */
} EthIf_Cfg_Cbk_Func_Table_t;

/*!
 * @brief           Mapping configuration structure for EthIf ports.
 */
typedef struct
{
    /*! Controller index. */
    uint8                               u8CtrlIdx; /* 1 byte */
    /*! Port index within controller context. */
    uint8                               u8CtrlPortIdx; /* 1 byte */
    /*! Transceiver index. */
    uint8                               u8TrcvIdx; /* 1 byte */
    /* 5 bytes padding */
    /* 8 bytes total */
} EthIf_Ctrl_Port_t;

/*!
 * @brief           General EthIf component configuration structure.
 */
typedef struct
{
    /*! Number of available configured ports for each configured switch. */
    uint8                               *pu8SwitchPorts; /* 8 bytes */
    /*! Pointer to array mapping available ports to controllers. Index of array
        corelates with index of port. Element of array is index in array
        of controllers function tables. */
    EthIf_Ctrl_Port_t                   *arPortMap; /* 8 bytes */
    /*! Number of unique configured controllers. */
    uint8                               u8Controllers; /* 1 byte */
    /*! Number of available configured ports. */
    uint8                               u8Ports; /* 1 byte */
    /*! Number of available configured transceivers. */
    uint8                               u8Transceivers; /* 1 byte */
    /*! Number of available configured switches. */
    uint8                               u8Switches; /* 1 byte */
    /* 4 bytes padding */
    /* 24 bytes total */
} EthIf_Config_t;

/*******************************************************************************
 * Data Types
 ******************************************************************************/
/* None */

/*******************************************************************************
 * Externs
 ******************************************************************************/

extern EthIf_Cfg_Ctrl_Func_Table_t arEthIfCfgFuncMap[];
extern EthIf_Cfg_Cbk_Func_Table_t rEthIfCfgCbkFuncMap;
extern EthIf_Config_t rEthIfCompConfig;
/* Last link status for change detection. */
extern EthTrcv_LinkStateType seLastLinkState[];
extern EthTrcv_ModeType aeTrcvsModes[];
extern EthTrcv_ModeType *apeSwitchPortsModes[];

/*******************************************************************************
 * API
 ******************************************************************************/
/* None */

/*******************************************************************************
 * Guard End
 ******************************************************************************/

#endif /* ETHIF_CFG_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
