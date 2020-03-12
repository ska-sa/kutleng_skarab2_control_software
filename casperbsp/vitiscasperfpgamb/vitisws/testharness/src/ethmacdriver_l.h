/*------------------------------------------------------------------------------
-- Legal & Copyright:   (c) 2018 Kutleng Engineering Technologies (Pty) Ltd    -
--                                                                             -
-- This program is the proprietary software of Kutleng Engineering Technologies-
-- and/or its licensors, and may only be used, duplicated, modified or         -
-- distributed pursuant to the terms and conditions of a separate, written     -
-- license agreement executed between you and Kutleng (an "Authorized License")-
-- Except as set forth in an Authorized License, Kutleng grants no license     -
-- (express or implied), right to use, or waiver of any kind with respect to   -
-- the Software, and Kutleng expressly reserves all rights in and to the       -
-- Software and all intellectual property rights therein.  IF YOU HAVE NO      -
-- AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, -
-- AND SHOULD IMMEDIATELY NOTIFY KUTLENG AND DISCONTINUE ALL USE OF THE        -
-- SOFTWARE.                                                                   -
--                                                                             -
-- Except as expressly set forth in the Authorized License,                    -
--                                                                             -
-- 1.     This program, including its structure, sequence and organization,    -
-- constitutes the valuable trade secrets of Kutleng, and you shall use all    -
-- reasonable efforts to protect the confidentiality thereof,and to use this   -
-- information only in connection with South African Radio Astronomy           -
-- Observatory (SARAO) products.                                               -
--                                                                             -
-- 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED     -
-- "AS IS" AND WITH ALL FAULTS AND KUTLENG MAKES NO PROMISES, REPRESENTATIONS  -
-- OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH       -
-- RESPECT TO THE SOFTWARE.  KUTLENG SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED-
-- WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A        -
-- PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET        -
-- ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE-
-- ENJOYMENT, QUIET POSSESSION USE OR PERFORMANCE OF THE SOFTWARE.             -
--                                                                             -
-- 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL KUTLENG OR -
-- ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT-
-- , OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO  -
-- YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF KUTLENG HAS BEEN       -
-- ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF -
-- THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR ZAR R1, WHICHEVER IS    -
-- GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF       -
-- ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.                                    -
-- --------------------------------------------------------------------------- -
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS                    -
-- PART OF THIS FILE AT ALL TIMES.                                             -
--=============================================================================-
-- Company          : Kutleng Dynamic Electronics Systems (Pty) Ltd            -
-- Author           : Benjamin Hector Hlophe                                   -
--                                                                             -
-- Design Name      : CASPER BSP                                               -
-- Module Name      : ethmacdriver                                             -
-- Project Name     : SKARAB2                                                  -
-- Target Devices   : N/A                                                      -
-- Tool Versions    : N/A                                                      -
-- Description      : This is the CPU interface to the 100G Ethernet MAC.      -
--                    The module defines access functions to and from the MAC. -
-- Dependencies     : none                                                     -
-- Revision History : V1.0 - Initial design                                    -
------------------------------------------------------------------------------*/
/******************************************************************************/
/**
*
* @file ethmacdriver_l.h
* @addtogroup ethernetxgmac_v1_0
* @{
*
* This header file contains identifiers and low-level driver functions/macros
* that can be used to access the device.  High-level driver functions are
* defined in ethmacdriver.h.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver     Who         Date     Changes
* ----- ---------- ---------- --------------------------------------------------
* 1.00a BH Hlophe  26/10/2019 First release
* </pre>
*
*******************************************************************************/
#ifndef SRC_ETHMACDRIVER_L_H_
#define SRC_ETHMACDRIVER_L_H_


#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/

#include "xil_types.h"
#include "xil_assert.h"
#include "xil_io.h"



/************************** Constant Definitions ****************************/

#define ETHERNETCORE_MM_S00_AXI_SLV_REG0_OFFSET 0
#define ETHERNETCORE_MM_S00_AXI_SLV_REG1_OFFSET 4
#define ETHERNETCORE_MM_S00_AXI_SLV_REG2_OFFSET 8
#define ETHERNETCORE_MM_S00_AXI_SLV_REG3_OFFSET 12
#define ETHERNETCORE_MM_S00_AXI_SLV_REG4_OFFSET 16
#define ETHERNETCORE_MM_S00_AXI_SLV_REG5_OFFSET 20
#define ETHERNETCORE_MM_S00_AXI_SLV_REG6_OFFSET 24
#define ETHERNETCORE_MM_S00_AXI_SLV_REG7_OFFSET 28
#define ETHERNETCORE_MM_S00_AXI_SLV_REG8_OFFSET 32
#define ETHERNETCORE_MM_S00_AXI_SLV_REG9_OFFSET 36
#define ETHERNETCORE_MM_S00_AXI_SLV_REG10_OFFSET 40
#define ETHERNETCORE_MM_S00_AXI_SLV_REG11_OFFSET 44
#define ETHERNETCORE_MM_S00_AXI_SLV_REG12_OFFSET 48
#define ETHERNETCORE_MM_S00_AXI_SLV_REG13_OFFSET 52
#define ETHERNETCORE_MM_S00_AXI_SLV_REG14_OFFSET 56
#define ETHERNETCORE_MM_S00_AXI_SLV_REG15_OFFSET 60
#define ETHERNETCORE_MM_S00_AXI_SLV_REG16_OFFSET 64
#define ETHERNETCORE_MM_S00_AXI_SLV_REG17_OFFSET 68
#define ETHERNETCORE_MM_S00_AXI_SLV_REG18_OFFSET 72
#define ETHERNETCORE_MM_S00_AXI_SLV_REG19_OFFSET 76
#define ETHERNETCORE_MM_S00_AXI_SLV_REG20_OFFSET 80
#define ETHERNETCORE_MM_S00_AXI_SLV_REG21_OFFSET 84
#define ETHERNETCORE_MM_S00_AXI_SLV_REG22_OFFSET 88
#define ETHERNETCORE_MM_S00_AXI_SLV_REG23_OFFSET 92
#define ETHERNETCORE_MM_S00_AXI_SLV_REG24_OFFSET 96
#define ETHERNETCORE_MM_S00_AXI_SLV_REG25_OFFSET 100
#define ETHERNETCORE_MM_S00_AXI_SLV_REG26_OFFSET 104
#define ETHERNETCORE_MM_S00_AXI_SLV_REG27_OFFSET 108
#define ETHERNETCORE_MM_S00_AXI_SLV_REG28_OFFSET 112
#define ETHERNETCORE_MM_S00_AXI_SLV_REG29_OFFSET 116
#define ETHERNETCORE_MM_S00_AXI_SLV_REG30_OFFSET 120
#define ETHERNETCORE_MM_S00_AXI_SLV_REG31_OFFSET 124


/* XGMAC register offsets */

/* Enable Bit register offset  */
#define XUL_MAC_ENABLE_REG_OFFSET  			0x028
/* Enable bit is bit[0] */
#define XUL_ENABLE_BIT_POSITION	   			0x00000001
/* Promiscuous register offset  */
#define XUL_MAC_PROMISCUOUS_REG_OFFSET  	0x028
/* Promiscuous bit is bit[1] */
#define XUL_PROMISCUOUS_BIT_POSITION	  	0x00000002
/* Statistics Reset register offset  */
#define XUL_MAC_STATISTICS_REG_OFFSET  		0x078
/* Statistics Reset bit is bit[0] */
#define XUL_STATISTICS_BIT_POSITION	  		0x00000001
/* Core type register offset */
#define XUL_CORE_TYPE_REG_OFFSET     	ETHERNETCORE_MM_S00_AXI_SLV_REG0_OFFSET
/* RX/TX buffer size register offset */
#define XUL_BUFFER_SIZE_REG_OFFSET		ETHERNETCORE_MM_S00_AXI_SLV_REG1_OFFSET
/* TX/RX Word size register offset */
#define XUL_TXRX_WORD_SIZE_REG_OFFSET	ETHERNETCORE_MM_S00_AXI_SLV_REG2_OFFSET
/* PHY control register high offset */
#define XUL_REG_PHY_CONTROLH_OFFSET		ETHERNETCORE_MM_S00_AXI_SLV_REG14_OFFSET
/* PHY control register low offset */
#define XUL_REG_PHY_CONTROLL_OFFSET		ETHERNETCORE_MM_S00_AXI_SLV_REG15_OFFSET
/* ARP cache size register offset */
#define XUL_ARP_SIZE_REG_OFFSET			ETHERNETCORE_MM_S00_AXI_SLV_REG16_OFFSET
/* TX packet rate register offset */
#define XUL_TX_PACKET_RATE_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG17_OFFSET
/* TX packet count register offset */
#define XUL_TX_PACKET_COUNT_REG_OFFSET	ETHERNETCORE_MM_S00_AXI_SLV_REG18_OFFSET
/* TX valid rate register offset */
#define XUL_TX_VALID_RATE_REG_OFFSET   	ETHERNETCORE_MM_S00_AXI_SLV_REG19_OFFSET
/* TX valid count register offset */
#define XUL_TX_VALID_COUNT_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG20_OFFSET
/* TX overflow count register offset */
#define XUL_TX_OVERFLOW_COUNT_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG21_OFFSET
/* TX overflow count register offset */
#define XUL_TX_ALMOST_FULL_COUNT_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG22_OFFSET
/* RX packet rate register offset */
#define XUL_RX_PACKET_RATE_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG23_OFFSET
/* RX packet count register offset */
#define XUL_RX_PACKET_COUNT_REG_OFFSET	ETHERNETCORE_MM_S00_AXI_SLV_REG24_OFFSET
/* RX valid rate register offset */
#define XUL_RX_VALID_RATE_REG_OFFSET   	ETHERNETCORE_MM_S00_AXI_SLV_REG25_OFFSET
/* RX valid count register offset */
#define XUL_RX_VALID_COUNT_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG26_OFFSET
/* RX overflow count register offset */
#define XUL_RX_OVERFLOW_COUNT_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG27_OFFSET
/* RX overflow count register offset */
#define XUL_RX_ALMOST_FULL_COUNT_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG28_OFFSET
/* RX bad packet count register offset */
#define XUL_RX_BAD_PACKET_COUNT_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG29_OFFSET
/* MAC Address high register offset */
#define XUL_MAC_HIGH_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG3_OFFSET
/* MAC Address low register offset */
#define XUL_MAC_LOW_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG4_OFFSET
/* Local IP Address  register offset */
#define XUL_LOCAL_IP_ADDRESS_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG5_OFFSET
/* Gateway IP Address  register offset */
#define XUL_GATEWAY_IP_ADDRESS_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG6_OFFSET
/* Multicast IP Address  register offset */
#define XUL_MULTICAST_IP_ADDRESS_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG7_OFFSET
/* Multicast IP Netmask  register offset */
#define XUL_MULTICAST_NETMASK_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG8_OFFSET
/* UDP Port register offset */
#define XUL_UDP_PORT_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG11_OFFSET
/* UDP Port mask register offset */
#define XUL_UDP_PORT_MASK_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG11_OFFSET
/* Ringh Buffer Slots register offset */
#define XUL_RING_BUFFER_SLOTS_REG_OFFSET   ETHERNETCORE_MM_S00_AXI_SLV_REG9_OFFSET


/****************************************************************************/
/**
*
* Write a value to a XGMAC register. A 32 bit write is performed.
*
* @param	BaseAddress is the register base address of the 100GMAC device.
* @param	RegOffset is the register offset from the base to write to.
* @param	Data is the data written to the register.
*
* @return	None.
*
* @note		C-style signature:
*		void XGMAC_WriteReg(u32 BaseAddress, u32 RegOffset,
*					u32 Data)
*
****************************************************************************/
#define XGMAC_WriteReg(BaseAddress, RegOffset, Data) \
	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/****************************************************************************/
/**
*
* Read a value from a XGMAC register. A 32 bit read is performed.
*
* @param	BaseAddress is the register base address of the 100GMAC device.
* @param	RegOffset is the register offset from the base to read from.
*
* @return	Data read from the register.
*
* @note		C-style signature:
*		u32 XGMAC_ReadReg(u32 BaseAddress, u32 RegOffset)
*
****************************************************************************/
#define XGMAC_ReadReg(BaseAddress, RegOffset) \
	Xil_In32((BaseAddress) + (RegOffset))

/****************************************************************************/
/**
*
* This function enables the 100GMAC by setting the gmac_enable bit to one.
* The function will read the entire 32 bit register first and only change
* the gmac_enable bit on the register.
* On RTOS based systems there must be a barrier between the register read and
* the register write to make this an atomic read modify operation to prevent
* sequential data coupling.
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_EnableXGMAC(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function disables the 100GMAC by setting the gmac_enable bit to zero.
* The function will read the entire 32 bit register first and only change
* the gmac_enable bit on the register.
* On RTOS based systems there must be a barrier between the register read and
* the register write to make this an atomic read modify operation to prevent
* sequential data coupling.
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_DisableXGMAC(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function sets the 100GMAC to receive all packets at the MAC level without
* filtering by MAC address by setting the gmac_promis bit to one.
* The function will read the entire 32 bit register first and only change
* the gmac_enable bit on the register.
* On RTOS based systems there must be a barrier between the register read and
* the register write to make this an atomic read modify operation to prevent
* sequential data coupling.
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_EnableXGMACPromiscuousMode(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function sets the 100GMAC to receive only packets addressed at the MAC
* level by filtering and only consuming packets addressed to the 100GMAC MAC
* address.
* The function will read the entire 32 bit register first and only change
* the gmac_enable bit on the register.
* On RTOS based systems there must be a barrier between the register read and
* the register write to make this an atomic read modify operation to prevent
* sequential data coupling.
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_DisableXGMACPromiscuousMode(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function resets the 100GMAC statistics counter registers to zero.
* The function will read the entire 32 bit register first and only change
* the gmac_counters_reset bit on the register.
* On RTOS based systems there must be a barrier between the register read and
* the register write to make this an atomic read modify operation to prevent
* sequential data coupling.
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_ResetStatisticsCounters(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function gets the core version and type by reading the core status
* register.
* The function will read the entire 32 bit register with the variable register
* bit fields.
*
* @param	BaseAddress is the base address of the device
*
* @return	CoreTypeRegister.
*
* @note		The register map is as follows:
* 			Register[07:00]:type[7:0]:8-bit Ethernet Type = 0x05
* 			Register[15:08]:rev[7:0] :8-bit Core Version  = 0x10
* 			Register[23:16]:CPU_RX_ENABLED[16]='1'Enabled,'0'Disabled
*			Register[31:24]:CPU_TX_ENABLED[24]='1'Enabled,'0'Disabled
******************************************************************************/
u32 XGMAC_GetCoreType(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function gets the transmit buffer maximum size.
* The CPU interface has a transmit buffer of 2048 bytes.
* The real amount of buffer that can be used is only according to Ethernet MTU
* which is 1522 bytes.
* The function will read the entire 32 bit register and extract the correct
* bit fields.
*
* @param	BaseAddress is the base address of the device
*
* @return	TransmitBufferMaximumSize.
*
* @note		The register map is as follows:
* 			Register[31:00]:TX_BUF_MAX[31:16]:RX_BUF_MAX[15:00]
******************************************************************************/
u16 XGMAC_GetTXBufferMaximumSize(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function gets the receive buffer maximum size.
* The CPU interface has a receive buffer of 2048 bytes.
* The real amount of buffer that can be used is only according to Ethernet MTU
* which is 1522 bytes.
* The function will read the entire 32 bit register and extract the correct
* bit fields.
*
* @param	BaseAddress is the base address of the device
*
* @return	ReceiveBufferMaximumSize.
*
* @note		The register map is as follows:
* 			Register[31:00]:TX_BUF_MAX[31:16]:RX_BUF_MAX[15:00]
******************************************************************************/
u16 XGMAC_GetRXBufferMaximumSize(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function read the 100GMAC PHY configuration registers (63-32).
*
* @param	BaseAddress is the base address of the device
*
* @return	PHYConfigRegisterH.
*
* @note		Do not change this register as it is highly dependent on the PHY
*           implementation in VHDL according the the MGT and board settings.
*
******************************************************************************/
u32 XGMAC_GetPHYStatusH(UINTPTR BaseAddress);
#define XGMAC_ReadPHYControlH(uBaseAddress) \
		XGMAC_GetPHYStatusH(uBaseAddress)

/****************************************************************************/
/**
*
* This function read the 100GMAC PHY configuration registers (31-0).
*
* @param	BaseAddress is the base address of the device
*
* @return	PHYConfigRegisterL.
*
* @note		Do not change this register as it is highly dependent on the PHY
*           implementation in VHDL according the the MGT and board settings.
*
******************************************************************************/
u32 XGMAC_GetPHYStatusL(UINTPTR BaseAddress);
#define XGMAC_ReadPHYControlL(uBaseAddress) \
		XGMAC_GetPHYStatusL(uBaseAddress)

/****************************************************************************/
/**
*
* This function reads the 100GMAC ARP cache size registers.
*
* @param	BaseAddress is the base address of the device
*
* @return	ARPSize.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetARPSize(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC TX word size registers.
*
* @param	BaseAddress is the base address of the device
*
* @return	TXWordSize.
*
* @note		None.
*
******************************************************************************/
u16 XGMAC_GetTXWordSize(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC RX word size registers.
*
* @param	BaseAddress is the base address of the device
*
* @return	RXWordSize.
*
* @note		None.
*
******************************************************************************/
u16 XGMAC_GetRXWordSize(UINTPTR BaseAddress);


/****************************************************************************/
/**
*
* This function reads the 100GMAC transmit packet rate statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	RXPacketRate.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetTXPacketRate(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC transmit packet count statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	RXPacketCount.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetTXPacketCount(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC transmit valid rate statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	TXValidRate.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetTXValidRate(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC transmit valid count statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	TXValidCount.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetTXValidCount(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC transmit overflow count statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	TXOverflowCount.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetTXOverflowCount(UINTPTR BaseAddress);


/****************************************************************************/
/**
*
* This function reads the 100GMAC transmit almost full count statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	TXAFullCount.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetTXAFullCount(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC receive packet rate statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	RXPacketRate.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetRXPacketRate(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC receive packet count statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	RXPacketCount.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetRXPacketCount(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC receive valid rate statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	RXValidRate.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetRXValidRate(UINTPTR BaseAddress);
/****************************************************************************/
/**
*
* This function reads the 100GMAC receive valid count statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	RXValidCount.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetRXValidCount(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC receive overflow count statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	RXOverflowCount.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetRXOverflowCount(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC receive almost full count statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	RXAFullCount.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetRXAFullCount(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC receive bad packet count statistic counter register
*
* @param	BaseAddress is the base address of the device
*
* @return	RXBadPacketCount.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetRXBadPacketCount(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC MAC address register
*
* @param	BaseAddress is the base address of the device
*
* @return	MACAddress.
*
* @note		None.
*
******************************************************************************/
u64 XGMAC_GetMACAddress(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function set the 100GMAC MAC address register
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_SetMACAddress(UINTPTR BaseAddress,u64 MACAddress);

/****************************************************************************/
/**
*
* This function set the 100GMAC MAC IP address register
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_SetLocalIPAddress(UINTPTR BaseAddress,u32 IPAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC IP address register
*
* @param	BaseAddress is the base address of the device
*
* @return	IP Address.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetLocalIPAddress(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function set the 100GMAC Gateway IP address register
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_SetGatewayIPAddress(UINTPTR BaseAddress,u32 IPAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC Gateway IP address register
*
* @param	BaseAddress is the base address of the device
*
* @return	Gateway IP Address.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetGatewayIPAddress(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function set the 100GMAC Multicast IP address register
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_SetMulticastIPAddress(UINTPTR BaseAddress,u32 IPAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC Multicast IP address register
*
* @param	BaseAddress is the base address of the device
*
* @return	Multicast IP Address.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetMulticastIPAddress(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function set the 100GMAC Multicast IP Netmask address register
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_SetMulticastIPMask(UINTPTR BaseAddress,u32 IPAddressMask);

/****************************************************************************/
/**
*
* This function reads the 100GMAC Multicast IP Netmask address register
*
* @param	BaseAddress is the base address of the device
*
* @return	Multicast IP Netmask.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetMulticastIPMask(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function reads the 100GMAC Server UDP Port register
*
* @param	BaseAddress is the base address of the device
*
* @return	UDPPort.
*
* @note		None.
*
******************************************************************************/
u16 XGMAC_GetUDPPort(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function set the 100GMAC UDP Port address register
*
* @param	BaseAddress is the base address of the device
* 			UDPPort is the base port to be written to the register
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_SetUDPPort(UINTPTR BaseAddress,u16 UDPPort);
/****************************************************************************/
/**
*
* This function reads the 100GMAC Server UDP Port MAsk register
*
* @param	BaseAddress is the base address of the device
*
* @return	UDPPortMask.
*
* @note		None.
*
******************************************************************************/
u16 XGMAC_GetUDPPortMask(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function set the 100GMAC UDP Port mask address register
*
* @param	BaseAddress is the base address of the device
* 			UDPPort is the base port to be written to the register
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_SetUDPPortMask(UINTPTR BaseAddress,u16 UDPPort);

/****************************************************************************/
/**
*
* This function set the 100GMAC PHY High register
*
* @param	BaseAddress is the base address of the device
*           PHYHigh is the PHY High register values
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_WritePHYControlH(UINTPTR BaseAddress,u32 PHYHigh);

/****************************************************************************/
/**
*
* This function set the 100GMAC PHY Low register
*
* @param	BaseAddress is the base address of the device
*           PHYHigh is the PHY Low register values
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_WritePHYControlL(UINTPTR BaseAddress,u32 PHYLow);

/****************************************************************************/
/**
*
* This function set the cpu transmit slot id.
*
* @param	BaseAddress is the base address of the device
* 			SlotID is the slot id to set
* On RTOS based systems there must be a barrier between the register read and
* the register write to make this an atomic read modify operation to prevent
* sequential data coupling.
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_SetTXSlotID(UINTPTR BaseAddress,u16 SlotID);

/****************************************************************************/
/**
*
* This function gets the cpu transmit slot id
*
* @param	BaseAddress is the base address of the device
*
* @return	TXSlotID.
*
* @note		None.
*
******************************************************************************/
u16 XGMAC_GetTXSlotID(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function set the cpu receive slot id.
*
* @param	BaseAddress is the base address of the device
* 			SlotID is the slot id to set
* On RTOS based systems there must be a barrier between the register read and
* the register write to make this an atomic read modify operation to prevent
* sequential data coupling.
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_SetRXSlotID(UINTPTR BaseAddress,u16 SlotID);


/****************************************************************************/
/**
*
* This function gets the cpu receive slot id
*
* @param	BaseAddress is the base address of the device
*
* @return	TXSlotID.
*
* @note		None.
*
******************************************************************************/
u16 XGMAC_GetRXSlotID(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function gets  the cpu transmit slot status.
*
* @param	BaseAddress is the base address of the device
* @return	TXSlotStatusBit.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetTXSlotStatus(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function gets the cpu receive slot status.
*
* @param	BaseAddress is the base address of the device
* @return	RXSlotStatusBit.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetRXSlotStatus(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function gets the cpu receive slot status.
*
* @param	BaseAddress is the base address of the device
* @return	RXNumberOfSlotsFilled.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetRXNumberOfSlotsFilled(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function gets  the cpu transmit slot status.
*
* @param	BaseAddress is the base address of the device
* @return	TXNumberOfSlotsFilled.
*
* @note		None.
*
******************************************************************************/
u32 XGMAC_GetTXNumberOfSlotsFilled(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function clear the cpu receive slot.
*
* @param	BaseAddress is the base address of the device
* 			SlotID is the slot id to set
* On RTOS based systems there must be a barrier between the register read and
* the register write to make this an atomic read modify operation to prevent
* sequential data coupling.
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_RXSlotClear(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function set the cpu transmit slot.
*
* @param	BaseAddress is the base address of the device
* 			SlotID is the slot id to set
* On RTOS based systems there must be a barrier between the register read and
* the register write to make this an atomic read modify operation to prevent
* sequential data coupling.
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_TXSlotSet(UINTPTR BaseAddress);

/****************************************************************************/
/**
*
* This function gets the cpu receive slot status.
*
* @param	BaseAddress is the base address of the device
* 			ARPEntryOffset is the offset of the ARP entry taken from the IP Address
* 			e.g. 192.168.0.14 (For 0x00E is the ARP Entry Offset)
* 			for multicast addresses the top bit is set e.g. 224.168.0.14 = 0x10E
* 			MACAddress is the MAC address to be inserted into the table
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_WriteARPCacheEntry(UINTPTR BaseAddress,u16 ARPEntryOffset,u64 MACAddress);

/****************************************************************************/
/**
*
* This function gets the cpu receive slot status.
*
* @param	BaseAddress is the base address of the device
* 			ARPEntryOffset is the offset of the ARP entry taken from the IP Address
* 			e.g. 192.168.0.14 (For 0x00E is the ARP Entry Offset)
* 			for multicast addresses the top bit is set e.g. 224.168.0.14 = 0x10E
* @return	MACAddress contained on the ARP Entry Offset
*
* @note		None.
*
******************************************************************************/
u64 XGMAC_ReadARPCacheEntry(UINTPTR BaseAddress,u16 ARPEntryOffset);
/****************************************************************************/
/**
*
* This function gets a raw Ethernet frame from the current packet receive slot.
*
* @param	BaseAddress is the base address of the device
* 			PacketBuffer is the pointer to the receive data array.
* @return	ReceiveStatus Number of raw bytes received
*
* @note		The data frame received will contain all Ethernet data except the
* 			Ethernet FCS which the CMAC will strip.
*
******************************************************************************/
u32 XGMAC_GetEthernetPacket(UINTPTR BaseAddress,u8 *PacketBuffer);
/****************************************************************************/
/**
*
* This function gets a raw Ethernet frame from the current packet receive slot.
*
* @param	BaseAddress is the base address of the device
* 			PacketBuffer is the pointer to the transmit data array holding
* 			the Ethernet frame.
* 			PacketNumberOfBytes the length of the Ethernet frame in bytes
* @return	SendStatus Number of raw bytes sent
*
* @note		The frame data sent must contain all data on an raw Ethernet frame
* 			but must omit the Ethernet FCS and the CMAC will insert the FCS.
*
******************************************************************************/
u32 XGMAC_SendEthernetPacket(UINTPTR BaseAddress,const u8 *PacketBuffer,const u32 PacketNumberOfBytes);

#ifdef __cplusplus
}
#endif

#endif /* SRC_ETHMACDRIVER_L_H_ end of protection macro */


/** @} */
