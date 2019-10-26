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

/** @} */
