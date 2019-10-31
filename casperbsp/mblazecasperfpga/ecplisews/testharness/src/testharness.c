/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * testharness.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "ethmacdriver_l.h"

int main()
{
	u64 U64Data;
	u64 MACAddress;
	u32 U32Data;
	u16 U16Data,u16Var;
    init_platform();

    /* Test Enable/Disable API*/
    /* Enable the device by setting register 10 bit 0. */
    XGMAC_EnableXGMAC(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("Device is Enabled\n\r");
    /* Disable the device by setting register 10 bit 0. */
    XGMAC_DisableXGMAC(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("Device is disabled\n\r");
    /* End Test Enable/Disable API*/


    /* Test Promiscuous mode Enable/Disable API*/
    /* Enable Promiscuous mode by setting register 10 bit 1.*/
    XGMAC_EnableXGMACPromiscuousMode(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("Device is Enabled\n\r");
    /* Disable Promiscuous mode by setting register 10 bit 1.*/
    XGMAC_DisableXGMACPromiscuousMode(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("Device is disabled\n\r");
    /* End Test Promiscuous Mode Enable/Disable API*/


    /* Test get core type API*/
    /* Enable the device by setting register 10 bit 0. */
    XGMAC_EnableXGMAC(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("Device is Enabled\n\r");
    U32Data = XGMAC_GetCoreType(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetCoreType - CoreType=0x%08x (TX/RX enabled)\n\r",U32Data);
    /* Disable the device by setting register 10 bit 0. */
    XGMAC_DisableXGMAC(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("Device is disabled\n\r");
    U32Data = XGMAC_GetCoreType(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetCoreType - CoreType=0x%08x (TX/RX disabled)\n\r",U32Data);



    /* Test get RX/TX buffer size API*/
    U16Data = XGMAC_GetTXBufferMaximumSize(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetTXBufferMaximumSize - TXBuffersize=%d\n\r",U16Data);
    U16Data = XGMAC_GetRXBufferMaximumSize(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetRXBufferMaximumSize - RXBuffersize=%d\n\r",U16Data);

    /* Test Write PHY configuration register API*/
    U32Data = 0x00000001;
    XGMAC_WritePHYControlH(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U32Data);
    xil_printf("XGMAC_WritePHYControlH - PHYStatusRegisterH=0x%08x\n\r",U32Data);
    U32Data = 0x00000002;
    XGMAC_WritePHYControlL(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U32Data);
    xil_printf("XGMAC_WritePHYControlL - PHYStatusRegisterL=0x%08x\n\r",U32Data);



    /* Test get PHY configuration register API*/
    U32Data = XGMAC_GetPHYStatusH(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetPHYStatusH - PHYStatusRegisterH=0x%08x\n\r",U32Data);
    U32Data = XGMAC_GetPHYStatusL(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetPHYStatusL - PHYStatusRegisterL=0x%08x\n\r",U32Data);

    /* Test Read PHY configuration register API*/
    U32Data = XGMAC_ReadPHYControlH(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_ReadPHYControlH - PHYStatusRegisterH=0x%08x\n\r",U32Data);
    U32Data = XGMAC_ReadPHYControlL(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_ReadPHYControlL - PHYStatusRegisterL=0x%08x\n\r",U32Data);


    /* Test Write PHY configuration register API*/
    U32Data = 0x00000000;
    XGMAC_WritePHYControlH(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U32Data);
    xil_printf("XGMAC_WritePHYControlH - PHYStatusRegisterH=0x%08x\n\r",U32Data);
    U32Data = 0x00000000;
    XGMAC_WritePHYControlL(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U32Data);
    xil_printf("XGMAC_WritePHYControlL - PHYStatusRegisterL=0x%08x\n\r",U32Data);

    /* Test get ARP Size register API*/
    U32Data = XGMAC_GetARPSize(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetARPSize - ARPSize=%d\n\r",U32Data);


    /* Test TX/RX Word size register API*/
    U16Data = XGMAC_GetTXWordSize(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetTXWordSize - TXWordSize=%d\n\r",U16Data);
    U16Data = XGMAC_GetRXWordSize(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetRXWordSize - RXWordSize=%d\n\r",U16Data);


    XGMAC_EnableXGMAC(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);

    /* Test get RX packet rate register API*/
    U32Data = XGMAC_GetRXPacketRate(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetRXPacketRate - RXPacketRate=%d\n\r",U32Data);

    /* Test get RX packet count register API*/
    U32Data = XGMAC_GetRXPacketCount(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetRXPacketCount - RXPacketCount=%d\n\r",U32Data);


    /* Test get RX valid rate register API*/
    U32Data = XGMAC_GetRXValidRate(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetRXValidRate - RXValidRate=%d\n\r",U32Data);


    /* Test get RX valid count register API*/
    U32Data = XGMAC_GetRXValidCount(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetRXValidCount - RXValidCount=%d\n\r",U32Data);

    /* Test get RX overflow count register API*/
    U32Data = XGMAC_GetRXOverflowCount(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetRXOverflowCount - RXOverflowCount=%d\n\r",U32Data);

    /* Test get RX almost full count register API*/
    U32Data = XGMAC_GetRXAFullCount(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetRXAFullCount - RXAFullCount=%d\n\r",U32Data);


    /* Test get RX bad packet count register API*/
    U32Data = XGMAC_GetRXBadPacketCount(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetRXBadPacketCount - RXBadPacketCount=%d\n\r",U32Data);



    /* Test get TX packet rate register API*/
    U32Data = XGMAC_GetTXPacketRate(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetTXPacketRate - TXPacketRate=%d\n\r",U32Data);

    /* Test get TX packet count register API*/
    U32Data = XGMAC_GetTXPacketCount(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetTXPacketCount - TXPacketCount=%d\n\r",U32Data);


    /* Test get TX valid rate register API*/
    U32Data = XGMAC_GetTXValidRate(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetTXValidRate - TXValidRate=%d\n\r",U32Data);


    /* Test get TX valid count register API*/
    U32Data = XGMAC_GetTXValidCount(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetTXValidCount - TXValidCount=%d\n\r",U32Data);

    /* Test get TX overflow count register API*/
    U32Data = XGMAC_GetTXOverflowCount(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetTXOverflowCount - TXOverflowCount=%d\n\r",U32Data);

    /* Test get TX almost full count register API*/
    U32Data = XGMAC_GetTXAFullCount(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetTXAFullCount - TXAFullCount=%d\n\r",U32Data);

    XGMAC_DisableXGMAC(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);

    /* Test set/get MAC address API*/
    U64Data = 0x0000000A35024192;
    XGMAC_SetMACAddress(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U64Data);
    U64Data = XGMAC_GetMACAddress(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);

    /* Test set/get local IP address API*/
    U32Data = 0xC0A8640A;/* 192.168.100.10*/
    XGMAC_SetLocalIPAddress(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U32Data);
    U32Data = XGMAC_GetLocalIPAddress(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetLocalIPAddress - LocalIPAddress=0x%08x\n\r",U32Data);

    /* Test set/get Gateway IP address API*/
    U32Data = 0xC0A86401;/* 192.168.100.1*/
    XGMAC_SetGatewayIPAddress(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U32Data);
    U32Data = XGMAC_GetGatewayIPAddress(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetGatewayIPAddress - GatewayIPAddress=0x%08x\n\r",U32Data);


    /* Test set/get Multicast IP address API*/
    U32Data = 0xE0A8640A;/* 224.168.100.10*/
    XGMAC_SetMulticastIPAddress(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U32Data);
    U32Data = XGMAC_GetMulticastIPAddress(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetMulticastIPAddress - MulticastIPAddress=0x%08x\n\r",U32Data);

    /* Test set/get Multicast IP address API*/
    U32Data = 0xFFFFFF00;/* 255.255.255.0*/
    XGMAC_SetMulticastIPMask(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U32Data);
    U32Data = XGMAC_GetMulticastIPMask(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetMulticastIPMask - MulticastIPMask=0x%08x\n\r",U32Data);

    /* Test set/get UDP Port API*/
    U16Data = 0x2000;/* 0x2000 */
    XGMAC_SetUDPPort(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U16Data);
    U16Data = XGMAC_GetUDPPort(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetUDPPort - UDPPort=0x%04hx\n\r",U16Data);

    /* Test set/get RXSlotID API*/
    for(U16Data = 0;U16Data<16;U16Data++)
    {
    	XGMAC_SetRXSlotID(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U16Data);
    	u16Var = XGMAC_GetRXSlotID(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    	xil_printf("XGMAC_GetRXSlotID - RXSlotID=%d\n\r",u16Var);
    	u16Var = XGMAC_GetRXSlotStatus(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    	xil_printf("XGMAC_GetRXSlotStatus - RXSlotStatus=%d\n\r",u16Var);
    }

    /* Test set/get TXSlotID API*/
    for(U16Data = 0;U16Data<16;U16Data++)
    {
    	XGMAC_SetTXSlotID(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U16Data);
    	u16Var = XGMAC_GetTXSlotID(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    	xil_printf("XGMAC_GetTXSlotID - TXSlotID=%d\n\r",u16Var);
    	u16Var = XGMAC_GetTXSlotStatus(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    	xil_printf("XGMAC_GetTXSlotStatus - TXSlotStatus=%d\n\r",u16Var);
        /* Test TXSlotset API*/
        XGMAC_TXSlotSet(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    	u16Var = XGMAC_GetTXSlotStatus(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    	xil_printf("XGMAC_GetTXSlotStatus - TXSlotStatus=%d\n\r",u16Var);
    	U32Data = XGMAC_GetTXNumberOfSlotsFilled(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    	xil_printf("XGMAC_GetTXNumberOfSlotsFilled - TXNumberOfSlotsFilled=%d\n\r",U32Data);
    }

    /* Test RXSlotClear API*/
    XGMAC_RXSlotClear(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);

    /* Test TXSlotset API*/
    XGMAC_TXSlotSet(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);

    /* Test set/get UDP Port MAsk API*/
    U16Data = 0xFF00;/* 0xFF00 */
    XGMAC_SetUDPPortMask(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR,U16Data);
    U16Data = XGMAC_GetUDPPortMask(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("XGMAC_GetUDPPortMask - UDPPortMask=0x%04hx\n\r",U16Data);

    MACAddress = 0x0000000A35024192-10;

    for(U16Data=0;U16Data<32;U16Data++)
    {
    	XGMAC_WriteARPCacheEntry(XPAR_ETHERNETCORE_MM_0_S03_AXI_BASEADDR,U16Data,MACAddress);
    	U64Data=XGMAC_ReadARPCacheEntry(XPAR_ETHERNETCORE_MM_0_S03_AXI_BASEADDR,U16Data);
    	MACAddress+=1;
    }

    /* Test Statistics Reset API*/
    XGMAC_ResetStatisticsCounters(XPAR_ETHERNETCORE_MM_0_S00_AXI_BASEADDR);
    xil_printf("Statistics registers cleared\n\r");
    /* End Test Statistics Reset  API*/


    cleanup_platform();
    return 0;
}
