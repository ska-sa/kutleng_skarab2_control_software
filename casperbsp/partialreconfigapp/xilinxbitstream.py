#! /usr/bin/python2.7

#-------------------------------------------------------------------------------
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
import time
from ctypes import *
prloaderlib = CDLL("./libxilinxbitstream.so")

def main():
	#Send the bitfile over UDP calling the C library function
	print("Clearing Current RM...\r\n")
	print("Initializing with PartialBlinker_i_greybox_partial\r\n")
	prloaderlib.configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu118/PartialBlinker_i_greybox_partial.bin")
	print("PartialBlinker_i_greybox_partial done\r\n")
	print("Waiting 10 seconds...\r\n")
	
	time.sleep(10.0)
	
	print("Initializing with PartialBlinker_i_partialblinker_partial\r\n")
	prloaderlib.configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu118/PartialBlinker_i_partialblinker_partial.bin")
	print("PartialBlinker_i_partialblinker_partial done\r\n")
	print("Waiting 10 Seconds...\r\n")
	
	time.sleep(10.0)
	print("Clearing Current RM...\r\n")
	print("Initializing with PartialBlinker_i_greybox_partial\r\n")
	prloaderlib.configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu118/PartialBlinker_i_greybox_partial.bin")
	print("PartialBlinker_i_greybox_partial done\r\n")
	print("Waiting 10 seconds...\r\n")
	
	time.sleep(10.0)
	
	print("Initializing with PartialBlinker_i_partialflasher_partial\r\n")
	retval = c_int(prloaderlib.configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu118/PartialBlinker_i_partialflasher_partial.bin"))
	print("PartialBlinker_i_partialflasher_partial done\r\n")
	
	print("Printing Debug Status Bits")
	print("STAT Register is = %08x" % retval.value)
	print("Mode pins M[2:0]=%0x" %((retval.value>>8)&0x00000007))
	print("CFG_STARTUP_PHASE="+str((retval.value>>18)&0x00000007))
	print("CFG_BUS_WIDTH="+str((retval.value>>25)&0x00000003))
	#Check the CRC Error bit
	if retval.value&0x00000001:
		print("CRC_ERROR = 1,CRC Error after config")
	else:
		print("CRC_ERROR = 0,No CRC Error after config")
	#Check the Decryptor bit
	if retval.value&0x00000002:
		print("DECRYPTOR_ENABLED = 1,Security set")
	else:
		print("DECRYPTOR_ENABLED = 0,Security not set")
	#Check the PLL lock bit
	if retval.value&0x00000004:
		print("MMCM_PLL_LOCKED = 1,PLLs are locked")
	else:
		print("MMCM_PLL_LOCKED = 0,PLLs are not locked")
	#Check the DCI Match bit
	if retval.value&0x00000008:
		print("DCI_MATCH_STATUS = 1,DCI has matched")
	else:
		print("DCI_MATCH_STATUS = 0,DCI has not matched")
	#Check the EOS bit
	if retval.value&0x00000010:
		print("EOS Status:1 = Finished")
	else:
		print("EOS Status:0 = Not finished")
	#Check the CFG_B bit
	if retval.value&0x00000020:
		print("GTS_CFG_B_STATUS = 1 all I/Os configured")
	else:
		print("GTS_CFG_B_STATUS = 0 all I/Os in highZ")
	#Check the GWE bit
	if retval.value&0x00000040:
		print("GWE_STATUS = 1 all FF/RAM are write enabled")
	else:
		print("GWE_STATUS = 0 all FF/RAM are write disabled")
	#Check the GHIGH bit
	if retval.value&0x00000080:
		print("GHIGH_B_STATUS = 1 deasserted")
	else:
		print("GHIGH_B_STATUS = 0 asserted")
		
	if retval.value&0x00000800:
		print("INIT_B_INTERNAL_SIGNAL_STATUS = 1")
		print("Inititalisation finished")
	else:
		print("INIT_B_INTERNAL_SIGNAL_STATUS = 0")
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
	
	print("Exiting\r\n")
	
	exit() 
main() 
