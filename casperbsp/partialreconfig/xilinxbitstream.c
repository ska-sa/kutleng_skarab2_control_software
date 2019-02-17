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
-- Engineer         : Benjamin Hector Hlophe                                   -
--                                                                             -
-- Design Name      : CASPER BSP                                               -
-- Module Name      : xilinxbitstream                                          -
-- Project Name     : SKARAB2                                                  -
-- Target Devices   : N/A                                                      -
-- Tool Versions    : N/A                                                      -
-- Description      : This module sends bitstream data over UDP for partial    -
--                    reconfiguration through ICAP on the prconfigcontroller.  -
--                    TODO                                                     -
--                    Must investiage using sliding window protocol to optimise-
--                    and improve data transfer speed.                         -
--                                                                             -
-- Dependencies     : none                                                     -
-- Revision History : V1.0 - Initial design                                    -
------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 

#define MAXLINE 2048
#define MAXSEND 1500-64

#define FRAME_WRITE 0xA5
#define DWORD_WRITE 0xDA

/*Data goes out in MSD first according to network byte ordering rules. */
#define C_PR_FRAME_START  0x5D400030


void SendPRData(int clientsocket, struct sockaddr_in serveraddr,uint8_t *packet,int length)
{
	int rlength;
	static char ibuffer[40];
	sendto(clientsocket, (const char *)packet, length,
	MSG_CONFIRM, (const struct sockaddr *) &serveraddr, 
	sizeof(serveraddr));

	recvfrom(clientsocket, (char *)ibuffer,10,
	MSG_WAITALL, (struct sockaddr *) &serveraddr,&rlength);
}


uint32_t configurepartialbitfile(uint32_t serverport, char *serverip,char *filename)
{
	FILE * 		BitFile;
	struct 		sockaddr_in 	serverAddr;
    ssize_t 	BytesReadSize;
	int 		clientSocket, portNum,i;
	uint8_t 	PRDwordPacket[10];
	static uint8_t 	PRFramePacket[398];
	static uint32_t 	FrameDword[98];
	uint32_t 	FileDWORD = 0;
	uint32_t 	SequenceCount = 0;

	/*Create UDP socket*/
	clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

	/*Configure settings in address struct*/
    memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverport);
	serverAddr.sin_addr.s_addr = inet_addr(serverip);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  


    /* Open the partial bitfile (binary)  to be sent to the ICAP
     * for partial reconfiguration. */
    BitFile = fopen(filename,"r+b");
    
    /* Check if and error occured while opening the bitfile.*/
    if (BitFile == NULL)
    {
		exit(EXIT_FAILURE);
	}
	
	fseek(BitFile,0L,SEEK_SET);
	

	/* Read biinary bitfile DWORD by DWORD. */
    while (!feof(BitFile)) 
    {
		/*Fetch 1 DWORD from the bitfile*/
		BytesReadSize = fread(&FileDWORD,sizeof(uint32_t),1,BitFile);

        if(FileDWORD == C_PR_FRAME_START)
		{
			/*The DWORD is a FRAME start command,fetch all 98 DWORDS,
			 *for the partial frame.
		     */
			FrameDword[0]=C_PR_FRAME_START;
			for(i=1;i<=97;i++)
			{
				/* Fetch all remaining 97 DWORDS from bitfile.*/
				if((BytesReadSize = fread(&FileDWORD,sizeof(uint32_t),1,BitFile)) != EOF)
				{
					FrameDword[i]=FileDWORD;
				}
			}
			/*Pack data properly byte wise on a stream for sending over UDP. */
			PRFramePacket[0] = (uint8_t)FRAME_WRITE;
			PRFramePacket[1] = (uint8_t)0x62;
			PRFramePacket[2] = (uint8_t)(0x000000FF&(SequenceCount>>24));
			PRFramePacket[3] = (uint8_t)(0x000000FF&(SequenceCount>>16));
			PRFramePacket[4] = (uint8_t)(0x000000FF&(SequenceCount>>8));
			PRFramePacket[5] = (uint8_t)(0x000000FF&(SequenceCount));
			/*Pack the remaining 97 DWORDs */
			for(i = 0;i<97;i++)
			{
				PRFramePacket[((i+1)*6)+3] = (uint8_t)(0x000000FF&FrameDword[i]);
				PRFramePacket[((i+1)*6)+2] = (uint8_t)(0x000000FF&((FrameDword[i])>>8));
				PRFramePacket[((i+1)*6)+1] = (uint8_t)(0x000000FF&((FrameDword[i])>>16));
				PRFramePacket[((i+1)*6)+0] = (uint8_t)(0x000000FF&((FrameDword[i])>>24));
			}
			/*Send the data over UDP.*/
			SendPRData(clientSocket,serverAddr,PRFramePacket,398);
		}
		else
		{
			/* Pack the DWORD. */
			PRDwordPacket[0] = (uint8_t)DWORD_WRITE;
			PRDwordPacket[1] = (uint8_t)0x01;
			PRDwordPacket[2] = (uint8_t)(0x000000FF&(SequenceCount>>24));
			PRDwordPacket[3] = (uint8_t)(0x000000FF&(SequenceCount>>16));
			PRDwordPacket[4] = (uint8_t)(0x000000FF&(SequenceCount>>8));
			PRDwordPacket[5] = (uint8_t)(0x000000FF&(SequenceCount));
			//Start of DWORD 
			PRDwordPacket[9] = (uint8_t)(0x000000FF&FileDWORD);
			PRDwordPacket[8] = (uint8_t)(0x000000FF&(FileDWORD>>8));
			PRDwordPacket[7] = (uint8_t)(0x000000FF&(FileDWORD>>16));
			PRDwordPacket[6] = (uint8_t)(0x000000FF&(FileDWORD>>24));
		    /*Send the data over UDP.*/
			SendPRData(clientSocket,serverAddr,PRDwordPacket,10);
		}
		/*Point to next sequence*/
		SequenceCount = SequenceCount+1;
    }

	
	/* Close bitfile after sending all packets using UDP protocol.*/
    fclose(BitFile);
	return 0;
}




