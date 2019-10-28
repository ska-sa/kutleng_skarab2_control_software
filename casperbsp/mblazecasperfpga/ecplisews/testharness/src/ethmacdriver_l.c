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
* @file ethmacdriver_l.c
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

/***************************** Include Files *********************************/

#include "ethmacdriver_l.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Prototypes ******************************/


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
void XGMAC_EnableXGMAC(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	/* Place a barrier here for RTOS based systems to prevent data coupling.*/
	lVar=XGMAC_ReadReg(BaseAddress,XUL_MAC_ENABLE_REG_OFFSET);
	lVar|=(XUL_ENABLE_BIT_POSITION);/* Set the enable bit*/
	XGMAC_WriteReg(BaseAddress, XUL_MAC_ENABLE_REG_OFFSET, lVar);
	/* Remove the barrier here.*/
}


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
void XGMAC_DisableXGMAC(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	/* Place a barrier here for RTOS based systems to prevent data coupling.*/
	lVar=XGMAC_ReadReg(BaseAddress,XUL_MAC_ENABLE_REG_OFFSET);
	lVar&=(!XUL_ENABLE_BIT_POSITION);/* Clear the enable bit*/
	XGMAC_WriteReg(BaseAddress, XUL_MAC_ENABLE_REG_OFFSET, lVar);
	/* Remove the barrier here.*/
}



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
void XGMAC_EnableXGMACPromiscuousMode(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	/* Place a barrier here for RTOS based systems to prevent data coupling.*/
	lVar=XGMAC_ReadReg(BaseAddress,XUL_MAC_PROMISCUOUS_REG_OFFSET);
	lVar|=(XUL_PROMISCUOUS_BIT_POSITION);/* Set the promiscuous bit*/
	XGMAC_WriteReg(BaseAddress, XUL_MAC_PROMISCUOUS_REG_OFFSET, lVar);
	/* Remove the barrier here.*/
}


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
void XGMAC_DisableXGMACPromiscuousMode(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	/* Place a barrier here for RTOS based systems to prevent data coupling.*/
	lVar=XGMAC_ReadReg(BaseAddress,XUL_MAC_PROMISCUOUS_REG_OFFSET);
	lVar&=(!XUL_PROMISCUOUS_BIT_POSITION);/* Clear the promiscuous bit*/
	XGMAC_WriteReg(BaseAddress, XUL_MAC_PROMISCUOUS_REG_OFFSET, lVar);
	/* Remove the barrier here.*/
}



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
void XGMAC_ResetStatisticsCounters(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	/* Place a barrier here for RTOS based systems to prevent data coupling.*/
	lVar=XGMAC_ReadReg(BaseAddress,XUL_MAC_STATISTICS_REG_OFFSET);
	lVar|=(XUL_STATISTICS_BIT_POSITION);/* Set the statistics reset bit*/
	XGMAC_WriteReg(BaseAddress, XUL_MAC_STATISTICS_REG_OFFSET, lVar);
	lVar=XGMAC_ReadReg(BaseAddress,XUL_MAC_STATISTICS_REG_OFFSET);
	lVar&=(!XUL_STATISTICS_BIT_POSITION);/* Clear the statistics reset bit*/
	XGMAC_WriteReg(BaseAddress, XUL_MAC_STATISTICS_REG_OFFSET, lVar);
	/* Remove the barrier here.*/
}


/****************************************************************************/
/**
*
* This function gets the core version and type by reading the core type status
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
u32 XGMAC_GetCoreType(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_CORE_TYPE_REG_OFFSET);
	return lVar;
}


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
u16 XGMAC_GetTXBufferMaximumSize(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_BUFFER_SIZE_REG_OFFSET);
	return (u16)((lVar>>16)&0x0000FFFF);
}

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
u16 XGMAC_GetRXBufferMaximumSize(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_BUFFER_SIZE_REG_OFFSET);
	return (u16)((lVar)&0x0000FFFF);
}


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
u32 XGMAC_GetPHYStatusH(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_REG_PHY_CONTROLH_OFFSET);
	return lVar;
}

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
u32 XGMAC_GetPHYStatusL(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_REG_PHY_CONTROLL_OFFSET);
	return lVar;
}

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
u32 XGMAC_GetARPSize(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_ARP_SIZE_REG_OFFSET);
	return lVar;
}


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
u16 XGMAC_GetTXWordSize(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_TXRX_WORD_SIZE_REG_OFFSET);
	return (u16)((lVar>>16)&0x0000FFFF);
}

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
u16 XGMAC_GetRXWordSize(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_TXRX_WORD_SIZE_REG_OFFSET);
	return (u16)(lVar&0x0000FFFF);
}

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
u32 XGMAC_GetTXPacketRate(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_TX_PACKET_RATE_REG_OFFSET);
	return lVar;
}

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
u32 XGMAC_GetTXPacketCount(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_TX_PACKET_COUNT_REG_OFFSET);
	return lVar;
}

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
u32 XGMAC_GetTXValidRate(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_TX_VALID_RATE_REG_OFFSET);
	return lVar;
}


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
u32 XGMAC_GetTXValidCount(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_TX_VALID_COUNT_REG_OFFSET);
	return lVar;
}

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
u32 XGMAC_GetTXOverflowCount(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_TX_OVERFLOW_COUNT_REG_OFFSET);
	return lVar;
}


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
u32 XGMAC_GetTXAFullCount(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_TX_ALMOST_FULL_COUNT_REG_OFFSET);
	return lVar;
}

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
u32 XGMAC_GetRXPacketRate(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_RX_PACKET_RATE_REG_OFFSET);
	return lVar;
}

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
u32 XGMAC_GetRXPacketCount(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_RX_PACKET_COUNT_REG_OFFSET);
	return lVar;
}

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
u32 XGMAC_GetRXValidRate(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_RX_VALID_RATE_REG_OFFSET);
	return lVar;
}


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
u32 XGMAC_GetRXValidCount(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_RX_VALID_COUNT_REG_OFFSET);
	return lVar;
}

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
u32 XGMAC_GetRXOverflowCount(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_RX_OVERFLOW_COUNT_REG_OFFSET);
	return lVar;
}


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
u32 XGMAC_GetRXAFullCount(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_RX_ALMOST_FULL_COUNT_REG_OFFSET);
	return lVar;
}

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
u32 XGMAC_GetRXBadPacketCount(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_RX_BAD_PACKET_COUNT_REG_OFFSET);
	return lVar;
}


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
u64 XGMAC_GetMACAddress(UINTPTR BaseAddress)
{
	volatile u32 lHigh,lLow;
	volatile u64 lVar;
	lHigh=XGMAC_ReadReg(BaseAddress,XUL_MAC_HIGH_REG_OFFSET);
	lLow=XGMAC_ReadReg(BaseAddress,XUL_MAC_LOW_REG_OFFSET);
	lVar = (u64)lHigh;
	lVar<<=32;
	lVar&=0xFFFFFFF00000000;
	lVar|=(((u64)lLow)&0x00000000FFFFFFFF);
	return lVar;
}

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
void XGMAC_SetMACAddress(UINTPTR BaseAddress,u64 MACAddress)
{
	volatile u32 lVar;
	lVar= (u32)(MACAddress&0x00000000FFFFFFFF);
	XGMAC_WriteReg(BaseAddress, XUL_MAC_LOW_REG_OFFSET, lVar);
	lVar= (u32)((MACAddress>>32)&0x00000000FFFFFFFF);
	XGMAC_WriteReg(BaseAddress,XUL_MAC_HIGH_REG_OFFSET,lVar);
}

/****************************************************************************/
/**
*
* This function set the 100GMAC IP address register
*
* @param	BaseAddress is the base address of the device
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void XGMAC_SetLocalIPAddress(UINTPTR BaseAddress,u32 IPAddress)
{
	XGMAC_WriteReg(BaseAddress, XUL_LOCAL_IP_ADDRESS_REG_OFFSET, IPAddress);
}

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
u32 XGMAC_GetLocalIPAddress(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_LOCAL_IP_ADDRESS_REG_OFFSET);
	return lVar;
}

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
void XGMAC_SetGatewayIPAddress(UINTPTR BaseAddress,u32 IPAddress)
{
	XGMAC_WriteReg(BaseAddress, XUL_GATEWAY_IP_ADDRESS_REG_OFFSET, IPAddress);
}

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
u32 XGMAC_GetGatewayIPAddress(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_GATEWAY_IP_ADDRESS_REG_OFFSET);
	return lVar;
}


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
void XGMAC_SetMulticastIPAddress(UINTPTR BaseAddress,u32 IPAddress)
{
	XGMAC_WriteReg(BaseAddress, XUL_MULTICAST_IP_ADDRESS_REG_OFFSET, IPAddress);
}

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
u32 XGMAC_GetMulticastIPAddress(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_MULTICAST_IP_ADDRESS_REG_OFFSET);
	return lVar;
}

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
void XGMAC_SetMulticastIPMask(UINTPTR BaseAddress,u32 IPAddressMask)
{
	XGMAC_WriteReg(BaseAddress, XUL_MULTICAST_NETMASK_REG_OFFSET, IPAddressMask);
}

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
u32 XGMAC_GetMulticastIPMask(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_MULTICAST_NETMASK_REG_OFFSET);
	return lVar;
}

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
u16 XGMAC_GetUDPPort(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	volatile u16 lUDPPort;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_UDP_PORT_REG_OFFSET);
	lUDPPort= (u16)(lVar&0x0000FFFF);
	return lUDPPort;
}


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
void XGMAC_SetUDPPort(UINTPTR BaseAddress,u16 UDPPort)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_UDP_PORT_REG_OFFSET);
	lVar&=0xFFFF0000;
	lVar|=(u32)(UDPPort&0x0000FFFF);
	XGMAC_WriteReg(BaseAddress, XUL_UDP_PORT_REG_OFFSET, lVar);
}


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
u16 XGMAC_GetUDPPortMask(UINTPTR BaseAddress)
{
	volatile u32 lVar;
	volatile u16 lUDPPort;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_UDP_PORT_REG_OFFSET);
	lUDPPort= (u16)((lVar>>16)&0x0000FFFF);
	return lUDPPort;
}


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
void XGMAC_SetUDPPortMask(UINTPTR BaseAddress,u16 UDPPort)
{
	volatile u32 lVar;
	lVar=XGMAC_ReadReg(BaseAddress,XUL_UDP_PORT_REG_OFFSET);
	lVar&=0x0000FFFF;
	lVar|=(u32)(UDPPort<<16&0xFFFF0000);
	XGMAC_WriteReg(BaseAddress, XUL_UDP_PORT_REG_OFFSET, lVar);
}


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
void XGMAC_WritePHYControlH(UINTPTR BaseAddress,u32 PHYHigh)
{
	XGMAC_WriteReg(BaseAddress, XUL_REG_PHY_CONTROLH_OFFSET, PHYHigh);
}

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
void XGMAC_WritePHYControlL(UINTPTR BaseAddress,u32 PHYLow)
{
	XGMAC_WriteReg(BaseAddress, XUL_REG_PHY_CONTROLL_OFFSET, PHYLow);
}

/** @} */
