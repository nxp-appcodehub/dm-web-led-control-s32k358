/*
 *   Copyright 2025 - 2026 NXP
 *
 *   NXP Proprietary. This software is owned or controlled by NXP
 *   and may only be used strictly in accordance with the applicable license terms.
 *   By expressly accepting such terms or by downloading, installing, activating
 *   and/or otherwise using the software, you are agreeing that you have read,
 *   and that you agree to comply with and are bound by, such license terms.
 *   If you do not agree to be bound by the applicable license terms,
 *   then you may not retain, install, activate or otherwise use the software.
 */

/*******************************************************************************
 * Guard Begin
 ******************************************************************************/

#ifndef ETHIF_H
#define ETHIF_H

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "BasicTypes.h"
#include "Eth_GeneralTypes.h"
#include "ComStack_Types.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*------------------[AUTOSAR release version identification]----------------*/

#if (defined ETHIF_AR_RELEASE_MAJOR_VERSION)
#error ETHIF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define ETHIF_AR_RELEASE_MAJOR_VERSION     4

#if (defined ETHIF_AR_RELEASE_MINOR_VERSION)
#error ETHIF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define ETHIF_AR_RELEASE_MINOR_VERSION     9

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
 
/*!
 * @brief           The function issues transmission confirmations in polling
 *                  mode. It checks also for transceiver state changes.
*/
void EthIf_MainFunctionTx(void);

/*!
 * @brief           The function checks for new received frames and issues
 *                  reception indications in polling mode.
*/
void EthIf_MainFunctionRx(void);

/*!
 * @brief           The function is polling different communication hardware
 *                  (Ethernet transceiver, Ethernet switch ports) related
 *                  information, e.g. link state, signal quality.
*/
void EthIf_MainFunctionState(void);

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
BufReq_ReturnType EthIf_ProvideTxBuffer(uint8 u8CtrlIdx,
                                        Eth_FrameType eFrameType,
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
void EthIf_EnableEgressTimeStamp(uint8 u8CtrlIdx,
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
Std_ReturnType EthIf_SetSwitchMgmtInfo(uint8 u8CtrlIdx,
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
Std_ReturnType EthIf_SwitchEnableTimeStamping(uint8 u8CtrlIdx,
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
Std_ReturnType EthIf_Transmit(uint8 u8CtrlIdx,
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
Std_ReturnType EthIf_GetTxMgmtObject(uint8 u8CtrlIdx,
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
Std_ReturnType EthIf_GetRxMgmtObject(uint8 u8CtrlIdx,
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
void EthIf_GetPhysAddr(uint8 u8CtrlIdx,
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
Std_ReturnType EthIf_GetEgressTimeStamp(uint8 u8CtrlIdx,
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
Std_ReturnType EthIf_GetIngressTimeStamp(uint8 u8CtrlIdx,
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
Std_ReturnType EthIf_UpdatePhysAddrFilter(uint8 u8CtrlIdx,
                                          const uint8 *cpu8PhysAddrPtr,
                                          Eth_FilterActionType eAction);

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
void EthIf_TrcvLinkStateChg(uint8 u8CtrlIdx,
                            EthTrcv_LinkStateType eTrcvLinkState);

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
void EthIf_RxIndication(uint8 u8CtrlIdx,
                        Eth_FrameType u16FrameType,
                        boolean bIsBroadcast,
                        const uint8 *cu8PhysAddrPtr,
                        const Eth_DataType *cu8DataPtr,
                        uint16 u16LenByte,
                        TimeTupleType* IngressTimeTuplePtr,
                        Eth_BufIdxType RxHandleId);

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
void EthIf_TxConfirmation(uint8 u8CtrlIdx,
                          Eth_BufIdxType u32BufferIndex,
                          Std_ReturnType eResult);

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
void EthIf_CtrlModeIndication(uint8 u8CtrlIdx,
                              Eth_ModeType eCtrlMode);

/*!
 * @brief           Eth If Transceiver mode indication.
 * @details         Called asynchronously when mode has been read out.
 *                  Triggered by previous Eth_SetTransceiverMode call.
 *                  Can directly be called within the trigger functions.
 *
 * @param[in]       u8TrcvIdx Index of the Ethernet transceiver within
 *                  the context of the Ethernet Interface.
 * @param[in]       eTrcvMode Notified Ethernet transceiver mode.
*/
void EthIf_TrcvModeIndication(uint8 u8TrcvIdx,
                              EthTrcv_ModeType eTrcvMode);

/*!
 * @brief           Eth If Switch Port mode indication.
 * @details         The EthIf shall determine the expected notifications based
 *                  on the EthSwtPort configuration. In case the EthSwtPort
 *                  references an EthTrcv the EthIf expects a notification from
 *                  the EthTrcv via API EthIf_TrcvModeIndication(). Otherwise
 *                  the EthIf expects a notification from the EthSwt via API
 *                  EthIf_SwitchPortModeIndication().
 *
 * @param[in]       u8SwitchIdx Index of the switch within the context of
 *                  the Ethernet Switch Driver.
 * @param[in]       u8SwitchPortIdx Index of the port at the addressed switch.
 * @param[in]       ePortMode Notified Ethernet Switch port mode.
*/
void EthIf_SwitchPortModeIndication(uint8 u8SwitchIdx,
                                    uint8 u8SwitchPortIdx,
                                    EthTrcv_ModeType ePortMode);

/*!
 * @brief           This function provides time value out of the HW registers
 *                  according to the capability of the HW.
 * @details
 * @param[in]       u8CtrlIdx Index of the addresses ETH controller.
 * @param[out]      timeQualPtr Quality of HW time stamp, e.g. based on current
 *                  drift.
 * @param[out]      timeStampPtr Current timestamp.
 * @return          Std_ReturnType
 * @retval          E_OK: Successful.
 * @retval          E_NOT_OK: Failed.
*/
Std_ReturnType EthIf_GetCurrentTime(uint8 u8CtrlIdx,
                                    Eth_TimeStampQualType* timeQualPtr,
                                    Eth_TimeStampType* timeStampPtr);

/*!
 * @brief           This function reads the current time of the timestamp clock
 *                  and the current time of the PHC in an atomic operation.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of
 *                  the Ethernet Interface which owns the clock unit.
 * @param[in]       u8ClkUnitIdx Index of the Clock Unit within the context of
 *                  the Ethernet Interface to provide the time tuple.
 * @param[out]      currentTimeTuplePtr Current time tuple with the (1) value of
 *                  the free-running clock used for timestamping, (2) value of
 *                  the adjustable PHC.
 * @return          Std_ReturnType
 * @retval          E_OK: Current time successfully retrieved.
 * @retval          E_NOT_OK: Current time could not be retrieved.
*/
Std_ReturnType EthIf_GetCurrentTimeTuple(uint8 u8CtrlIdx,
                                         uint8 u8ClkUnitIdx,
                                         TimeTupleType* currentTimeTuplePtr);

/*!
 * @brief           This function sets the absolute time of the PHC.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of
 *                  the Ethernet Interface which owns the clock unit.
 * @param[in]       u8ClkUnitIdx Index of the Clock Unit within the context of
 *                  the Ethernet Interface to provide the time tuple.
 * @param[in]       timeStampPtr Time value to which the PHC shall be set.
 * @return          Std_ReturnType
 * @retval          E_OK: PHC successfully set.
 * @retval          E_NOT_OK: PHC could not be set.
*/
Std_ReturnType EthIf_SetPhcTime(uint8 u8CtrlIdx,
                                uint8 u8ClkUnitIdx,
                                const TimeStampType* timeStampPtr);

/*!
 * @brief           This function returns the current time value out of the HW
 *                  registers of the PHC.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of
 *                  the Ethernet Interface which owns the clock unit.
 * @param[in]       u8ClkUnitIdx Index of the Clock Unit within the context of
 *                  the Ethernet Interface to provide the time tuple.
 * @param[in]       timeQualPtr Quality of HW time stamp, e.g. based on current
 *                  drift.
 * @param[in]       timeStampPtr Current timestamp.
 * @return          Std_ReturnType
 * @retval          E_OK: PHC value successfully retrieved.
 * @retval          E_NOT_OK: PHC value could not be retrieved.
*/
Std_ReturnType EthIf_GetPhcTime(uint8 u8CtrlIdx,
                                uint8 u8ClkUnitIdx,
                                TimeStampQualType* timeQualPtr,
                                TimeStampType* timeStampPtr);

/*!
 * @brief           This function sets PHC parameters to adapt rate and offset
 *                  of the PHC.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of
 *                  the Ethernet Interface which owns the clock unit.
 * @param[in]       u8ClkUnitIdx Index of the Clock Unit within the context of
 *                  the Ethernet Interface to provide the time tuple.
 * @param[in]       s32RateDeviation Rate deviation (resolution: 2-41), by which
 *                  the PHC is requested.
 * @param[in]       s32Offset Time offset, by which the PHC is requested to be
 *                  updated.
 * @return          Std_ReturnType
 * @retval          E_OK: PHC successfully set.
 * @retval          E_NOT_OK: PHC could not be set.
*/
Std_ReturnType EthIf_SetPhcCorrection(uint8 u8CtrlIdx,
                                      uint8 u8ClkUnitIdx,
                                      sint32 s32RateDeviation,
                                      sint32 s32Offset);

/*!
 * @brief           This function enables/disables the generation of a PPS
 *                  signal.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context of
 *                  the Ethernet Interface which owns the clock unit.
 * @param[in]       u8ClkUnitIdx Index of the Clock Unit within the context of
 *                  the Ethernet Interface to provide the time tuple.
 * @param[in]       bSignalMode TRUE: PPS signal generation is enabled,
 *                              FALSE: PPS signal generation is disabled.
 * @return          Std_ReturnType
 * @retval          E_OK: PPS signal generation successfully enabled/disabled.
 * @retval          E_NOT_OK: Failed to enable/disable PPS signal generation.
*/
Std_ReturnType EthIf_SetPpsSignalMode(uint8 u8CtrlIdx,
                                      uint8 u8ClkUnitIdx,
                                      boolean bSignalMode);

/*!
 * @brief           Enables / disables the indexed controller.
 * @details
 * @param[in]       u8CtrlIdx Index of Ethernet Controller within the context
 *                            of the Ethernet Interface.
 * @param[in]       eCtrlMode ETH_MODE_DOWN: disable the controller
 *                            ETH_MODE_ACTIVE: enable the controller
 *                            ETH_MODE_ACTIVE_WITH_WAKEUP_REQUEST: enable the
 *                              controller and request a wake-up on the network.
 *                            ETH_MODE_TX_OFFLINE: disable transmission
 *                              handling in Eth If. Please note, the according
 *                              Ethernet controller is not affected.
 * @return          Std_ReturnType
 * @retval          E_OK: Success.
 * @retval          E_NOT_OK: CtrlMode could not be changed.
*/
Std_ReturnType EthIf_SetControllerMode(uint8 u8CtrlIdx,
                                       Eth_ModeType eCtrlMode);

/*******************************************************************************
 * Guard End
 ******************************************************************************/

#endif /* ETHIF_H */

/*******************************************************************************
 * EOF
 ******************************************************************************/
