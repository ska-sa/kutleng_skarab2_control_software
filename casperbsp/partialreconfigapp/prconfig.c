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
	sendto(clientsocket, (const char *)packet, length,
	MSG_CONFIRM, (const struct sockaddr *) &serveraddr, 
	sizeof(serveraddr));

}


uint32_t configurepartialbitfile(uint32_t serverport, char *serverip,char *filename)
{
	FILE * 		BitFile;
	struct 		sockaddr_in 	serverAddr;
    ssize_t 	BytesReadSize;
	int 		clientSocket,i,PacketIndex;
	static uint8_t 	PRFramePacket[2048];
	static uint32_t 	FrameDword[512];
	uint32_t 	FileDWORD = 0;
	uint32_t 	SequenceCount = 0;
	uint32_t 	PacketDwordLength = 232;

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
		}
		/*Point to next sequence*/
		SequenceCount = SequenceCount+1;
    }
	
	/* Close bitfile after sending all packets using UDP protocol.*/
    fclose(BitFile);
	return 0;
}



int main() 
{
	printf("Clearing Current RM...\r\n");
	printf("Initializing with PartialBlinker_i_greybox_partial\r\n");
	configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu118/PartialBlinker_i_greybox_partial.bin"); 
	printf("PartialBlinker_i_greybox_partial done\r\n");
	printf("Waiting 10 seconds...\r\n");

	sleep(10);
	
	printf("Initializing with PartialBlinker_i_partialblinker_partial\r\n");
	configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu118/PartialBlinker_i_partialblinker_partial.bin"); 
	printf("PartialBlinker_i_partialblinker_partial done\r\n");
	printf("Waiting 10 Seconds...\r\n");
	
	sleep(10);

	printf("Clearing Current RM...\r\n");
	
	printf("Initializing with PartialBlinker_i_greybox_partial\r\n");
	configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu118/PartialBlinker_i_greybox_partial.bin"); 
	printf("PartialBlinker_i_greybox_partial done\r\n");
	printf("Waiting 10 seconds...\r\n");

	sleep(10);

	
	printf("Initializing with PartialBlinker_i_partialflasher_partial\r\n");
	configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu118/PartialBlinker_i_partialflasher_partial.bin"); 
	printf("PartialBlinker_i_partialflasher_partial done\r\n");
	printf("Exiting\r\n");
    return 0;
}
