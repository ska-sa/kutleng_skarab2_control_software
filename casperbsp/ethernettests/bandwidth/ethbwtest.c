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

#define DFRAME_WRITE 0xAD



void SendPacketData(int clientsocket, struct sockaddr_in serveraddr,uint8_t *packet,int length)
{
	sendto(clientsocket, (const char *)packet, length,
	MSG_CONFIRM, (const struct sockaddr *) &serveraddr, 
	sizeof(serveraddr));

}


uint32_t startbandwidthtest(uint32_t serverport, char *serverip,char *filename)
{
	FILE * 		DataFile;
	struct 		sockaddr_in 	serverAddr;
    ssize_t 	BytesReadSize;
	int 		clientSocket,i,PacketIndex;
	static uint8_t 	FramePacket[2048];
	static uint32_t 	DataDword[512];
	uint32_t 	FileDWORD = 0;
	uint32_t 	SequenceCount = 0;
	uint32_t 	dlength = 370;
	

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
    DataFile = fopen(filename,"r+b");
    
    /* Check if and error occured while opening the bitfile.*/
    if (DataFile == NULL)
    {
		exit(EXIT_FAILURE);
	}
	
	fseek(DataFile,0L,SEEK_SET);
	

	/* Read biinary bitfile DWORD by DWORD. */
	dlength =354 ;//244;
    while (!feof(DataFile)) 
    {

		{
			for(i=0;i<dlength;i++)
			{
				/* Fetch all remaining <246 DWORDS from bitfile.*/
				if((BytesReadSize = fread(&FileDWORD,sizeof(uint32_t),1,DataFile)) != EOF)
				{
					DataDword[i]=FileDWORD;
				}

				if(feof(DataFile))
				{
					dlength = (i+1);
				 	/*Break the for loop*/
				    //return 0;
					break;
				}
			}
			/*Pack data properly byte wise on a stream for sending over UDP. */
			FramePacket[0] = (uint8_t)DFRAME_WRITE;
			FramePacket[1] = (uint8_t)(dlength&0x000000FF);
			FramePacket[2] = (uint8_t)(0x000000FF&(SequenceCount>>24));
			FramePacket[3] = (uint8_t)(0x000000FF&(SequenceCount>>16));
			FramePacket[4] = (uint8_t)(0x000000FF&(SequenceCount>>8));
			FramePacket[5] = (uint8_t)(0x000000FF&(SequenceCount));
			/*Pack the remaining <246 DWORDs */
			for(i = 0,PacketIndex=6;i<dlength;i++,PacketIndex+=4)
			{
				FramePacket[(PacketIndex)+0] = (uint8_t)(0x000000FF&DataDword[i]);
				FramePacket[(PacketIndex)+1] = (uint8_t)(0x000000FF&((DataDword[i])>>8));
				FramePacket[(PacketIndex)+2] = (uint8_t)(0x000000FF&((DataDword[i])>>16));
				FramePacket[(PacketIndex)+3] = (uint8_t)(0x000000FF&((DataDword[i])>>24));
			}
			/*Send the data over UDP.*/
			for (;;)
				SendPacketData(clientSocket,serverAddr,FramePacket,((dlength*4) + 6));
		}
		SequenceCount = SequenceCount+1;
    }

	
	/* Close bitfile after sending all packets using UDP protocol.*/
    fclose(DataFile);
	return 0;
}



int main() 
{
	printf("Initializing with testfile\r\n");
	startbandwidthtest(10000,"192.168.100.10", "testfile.bin"); 
	printf("Starting Bandwith test using testfile.bin done\r\n");
	printf("Exiting\r\n");
    return 0;
}
