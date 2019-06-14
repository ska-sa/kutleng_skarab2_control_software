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
	prloaderlib.configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu118/PartialBlinker_i_partialblinker_partial.bin")
	exit() 
main() 
