/*------------------------------------------------------------------------------
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
 

#define DFRAME_WRITE 0xAD

void SendPRData(int clientsocket, struct sockaddr_in serveraddr,uint8_t *packet,int length)
{
	
	int rlength;
	static char ibuffer[12];
    uint32_t rval;
	sendto(clientsocket, (const char *)packet, length,
	MSG_CONFIRM, (const struct sockaddr *) &serveraddr, 
	sizeof(serveraddr));
	/*
	 * This is for value status return
	recvfrom(clientsocket, (char *)ibuffer,10,
	MSG_WAITALL, (struct sockaddr *) &serveraddr,&rlength);
    rval = ((((ibuffer[6])&0x000000FF)<<24)&0xFF000000);
    rval |= ((((ibuffer[7])&0x000000FF)<<16)&0x00FF0000);
    rval |= ((((ibuffer[8])&0x000000FF)<<8)&0x0000FF00);
    rval |= ((((ibuffer[9])&0x000000FF))&0x000000FF);
    return rval;
	*/
}


uint32_t configurepartialbitfile(uint32_t serverport, char *serverip,char *filename)
{
	FILE * 		BitFile;
	struct 		sockaddr_in 	serverAddr;
    ssize_t 	BytesReadSize;
	int 		clientSocket,i,PacketIndex;
	/*Use malloc here else will blow the heap*/
	uint8_t 	*PRFramePacket;//[2048];
	uint32_t 	*FrameDword;//[512];	
	uint32_t 	FileDWORD = 0;
	uint32_t 	SequenceCount = 0;
	uint32_t 	PacketDwordLength = 232;
    uint32_t    iretval;
	
	PRFramePacket = (uint8_t *) malloc(2048);
	if(PRFramePacket == NULL) 
	{
		/*There was an error in allocating the memory.*/
		return -1;
	}
	
	FrameDword = (uint32_t *) malloc(sizeof(uint32_t)*512);
	if(FrameDword == NULL) 
	{
		/*There was an error in allocating the memory.*/
		return -1;
	}

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
	
	iretval = 0;

	/* Read binary bitfile DWORD by DWORD. */
	/*Pack 960 bytes and dont send more than 1024 bytes*/
	PacketDwordLength =232;
    while (!feof(BitFile)) 
    {

		{
			for(i=0;i<PacketDwordLength;i++)
			{
				/* Fetch all remaining <246 DWORDS from bitfile.*/
				if((BytesReadSize = fread(&FileDWORD,sizeof(uint32_t),1,BitFile)) != EOF)
				{
					FrameDword[i]=FileDWORD;
				}

				if(feof(BitFile))
				{
					PacketDwordLength = (i+1);
				 	/*Break the for loop*/
					break;
				}
			}
			/*Pack data properly byte wise on a stream for sending over UDP. */
			PRFramePacket[0] = (uint8_t)DFRAME_WRITE;
			PRFramePacket[1] = (uint8_t)(PacketDwordLength&0x000000FF);
			PRFramePacket[2] = (uint8_t)(0x000000FF&(SequenceCount>>24));
			PRFramePacket[3] = (uint8_t)(0x000000FF&(SequenceCount>>16));
			PRFramePacket[4] = (uint8_t)(0x000000FF&(SequenceCount>>8));
			PRFramePacket[5] = (uint8_t)(0x000000FF&(SequenceCount));
			/*Pack the remaining <246 DWORDs */
			for(i = 0,PacketIndex=6;i<PacketDwordLength;i++,PacketIndex+=4)
			{
				PRFramePacket[(PacketIndex)+3] = (uint8_t)(0x000000FF&FrameDword[i]);
				PRFramePacket[(PacketIndex)+2] = (uint8_t)(0x000000FF&((FrameDword[i])>>8));
				PRFramePacket[(PacketIndex)+1] = (uint8_t)(0x000000FF&((FrameDword[i])>>16));
				PRFramePacket[(PacketIndex)+0] = (uint8_t)(0x000000FF&((FrameDword[i])>>24));
			}
			/*Send the data over UDP.*/
			SendPRData(clientSocket,serverAddr,PRFramePacket,((PacketDwordLength*4) + 6));
			#if 0
			/*This is for testing read errors*/
			/*Test to see if we dont have errors*/
			if (((retval&0x000000F0)==0x00000050) || ((retval&0x000000F0)==0x00000010))
			{
				/*This is an CFGError condition, break and return STAT register*/
				break;
			}
			#endif
		}
		/*Point to next sequence*/
		SequenceCount = SequenceCount+1;
	}
	#if 0
	for(j = 0; j < 4; j++)
	{
		/*Perform readback of status here*/
		for(i = 0; i < 14;i++)
		{
			FileDWORD = lCommandArray[i];
			if(FileDWORD != 0)
			{
				PRDwordPacket[0] = (uint8_t)DWORD_WRITE;
			}else
			{
				PRDwordPacket[0] = (uint8_t)DWORD_READ;
			}
			/* Pack the DWORD. */
			PRDwordPacket[1] = (uint8_t)0x01;
			PRDwordPacket[2] = (uint8_t)(0x000000FF&(SequenceCount>>24));
			PRDwordPacket[3] = (uint8_t)(0x000000FF&(SequenceCount>>16));
			PRDwordPacket[4] = (uint8_t)(0x000000FF&(SequenceCount>>8));
			PRDwordPacket[5] = (uint8_t)(0x000000FF&(SequenceCount));
			//Start of DWORD
			PRDwordPacket[6] = (uint8_t)(0x000000FF&FileDWORD);
			PRDwordPacket[7] = (uint8_t)(0x000000FF&(FileDWORD>>8));
			PRDwordPacket[8] = (uint8_t)(0x000000FF&(FileDWORD>>16));
			PRDwordPacket[9] = (uint8_t)(0x000000FF&(FileDWORD>>24));
			/*Send the data over UDP.*/
			retval = SendPRData(clientSocket,serverAddr,PRDwordPacket,10);
			if(FileDWORD == 0x30008001)
			{
				iretval = retval;
			}
				SequenceCount = SequenceCount + 1;
		}
	}
	#endif
	
	/* Free all malloc memory here.*/
	free(PRFramePacket);
	free(FrameDword);
	/* Close bitfile after sending all packets using UDP protocol.*/
    fclose(BitFile);
	return iretval;
}
