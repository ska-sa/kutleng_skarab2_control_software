#! /usr/bin/python2.7

#-------------------------------------------------------------------------------
#- Legal & Copyright:   (c) 2018 Kutleng Engineering Technologies (Pty) Ltd    - 
#-                                                                             -
#- This program is the proprietary software of Kutleng Engineering Technologies-
#- and/or its licensors, and may only be used, duplicated, modified or         -
#- distributed pursuant to the terms and conditions of a separate, written     -
#- license agreement executed between you and Kutleng (an "Authorized License")-
#- Except as set forth in an Authorized License, Kutleng grants no license     -
#- (express or implied), right to use, or waiver of any kind with respect to   -
#- the Software, and Kutleng expressly reserves all rights in and to the       -
#- Software and all intellectual property rights therein.  IF YOU HAVE NO      -
#- AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, -
#- AND SHOULD IMMEDIATELY NOTIFY KUTLENG AND DISCONTINUE ALL USE OF THE        -
#- SOFTWARE.                                                                   -
#-                                                                             -
#- Except as expressly set forth in the Authorized License,                    -
#-                                                                             -
#- 1.     This program, including its structure, sequence and organization,    -
#- constitutes the valuable trade secrets of Kutleng, and you shall use all    -
#- reasonable efforts to protect the confidentiality thereof,and to use this   -
#- information only in connection with South African Radio Astronomy           -
#- Observatory (SARAO) products.                                               -
#-                                                                             -
#- 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED     -
#- "AS IS" AND WITH ALL FAULTS AND KUTLENG MAKES NO PROMISES, REPRESENTATIONS  -
#- OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH       -
#- RESPECT TO THE SOFTWARE.  KUTLENG SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED-
#- WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A        -
#- PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET        -
#- ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE-
#- ENJOYMENT, QUIET POSSESSION USE OR PERFORMANCE OF THE SOFTWARE.             -
#-                                                                             -
#- 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL KUTLENG OR -
#- ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT-
#- , OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO  -
#- YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF KUTLENG HAS BEEN       -
#- ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF -
#- THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR ZAR R1, WHICHEVER IS    -
#- GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF       -
#- ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.                                    -
#- --------------------------------------------------------------------------- -
#- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS                    -
#- PART OF THIS FILE AT ALL TIMES.                                             -
#-=============================================================================-
#- Company          : Kutleng Dynamic Electronics Systems (Pty) Ltd            -
#- Engineer         : Benjamin Hector Hlophe                                   -
#-                                                                             -
#- Design Name      : CASPER BSP                                               -
#- Module Name      : xilinxbitstream                                          -
#- Project Name     : SKARAB2                                                  -
#- Target Devices   : N/A                                                      -
#- Tool Versions    : N/A                                                      -
#- Description      : This module sends bitstream data over UDP for partial    -
#-                    reconfiguration through ICAP on the prconfigcontroller.  -
#-                    TODO                                                     -
#-                    Must investiage using sliding window protocol to optimise-
#-                    and improve data transfer speed.                         -
#-                                                                             -
#- Dependencies     : none                                                     -
#- Revision History : V1.0 - Initial design                                    -
#-------------------------------------------------------------------------------

from ctypes import *
prloaderlib = CDLL("./libxilinxbitstream.so")

def main():
	#Send the bitfile over UDP calling the C library function
	retval = c_int(prloaderlib.configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu1525/PartialBlinker_i_partialblinker_partial.bin"))
	print("STAT Register is = %08x" % retval.value)
	#Check the CRC Error bit
	if retval.value&0x00000001:
		print("CRC Error after config")
	else:
		print("No CRC Error after config")
	#Check the EOS bit
	if retval.value&0x00000010:
		print("EOS Status:1 = Finished")
	else:
		print("EOS Status:0 = Not finished")
	print("Mode pins M[2:0]=%0x" %((retval.value>>8)&0x00000007))
	print("CFG_STARTUP_PHASE="+str((retval.value>>18)&0x00000007))
	print("CFG_BUS_WIDTH="+str((retval.value>>25)&0x00000003))
	if retval.value&0x00000800:
		print("Inititalisation finished")
	else:
		print("Initialisation not finished")
	if retval.value&0x00001000:
		print("INIT_B:1 High")
	else:
		print("INIT_B:0 Low")
	if retval.value&0x00002000:
		print("INIT_DONE:1 High")
	else:
		print("INIT_DONE:0 Low")
	if retval.value&0x00004000:
		print("DONE:1 High")
	else:
		print("DONE:0 Low")
	if retval.value&0x00008000:
		print("IDCODE ERROR:1 High")
	else:
		print("IDCODE NO ERROR:0 Low")
	if retval.value&0x00010000:
		print("SECURITY ERROR:1 High")
	else:
		print("SECURITY NO ERROR:0 Low")
	if retval.value&0x00020000:
		print("Device Over Temperature:1 High")
	else:
		print("Device Temperature:0 Ok")
	exit() 
main() 
