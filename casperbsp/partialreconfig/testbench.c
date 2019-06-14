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

/*Data goes out in MSD first according to network byte ordering rules. */
#define C_PR_FRAME_START  0x5D400030


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
	int 		clientSocket,i;
	uint8_t 	PRDwordPacket[10];
	static uint8_t 	PRFramePacket[1024+32];
	static uint32_t 	FrameDword[98];
	uint32_t 	FileDWORD = 0;
	uint32_t 	SequenceCount = 0;
	uint32_t 	dlength = 244;

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
	dlength = 244;
    while (!feof(BitFile)) 
    {

		{
			for(i=0;i<dlength;i++)
			{
				/* Fetch all remaining <246 DWORDS from bitfile.*/
				if((BytesReadSize = fread(&FileDWORD,sizeof(uint32_t),1,BitFile)) != EOF)
				{
					FrameDword[i]=FileDWORD;
				}

				if(feof(BitFile))
				{
					dlength = (i+1);
				 	/*Break the for loop*/
					break;
				}
			}
			/*Pack data properly byte wise on a stream for sending over UDP. */
			PRFramePacket[0] = (uint8_t)DFRAME_WRITE;
			PRFramePacket[1] = (uint8_t)(dlength&0x000000FF);
			PRFramePacket[2] = (uint8_t)(0x000000FF&(SequenceCount>>24));
			PRFramePacket[3] = (uint8_t)(0x000000FF&(SequenceCount>>16));
			PRFramePacket[4] = (uint8_t)(0x000000FF&(SequenceCount>>8));
			PRFramePacket[5] = (uint8_t)(0x000000FF&(SequenceCount));
			/*Pack the remaining <246 DWORDs */
			for(i = 0;i<dlength;i++)
			{
				PRFramePacket[((i+1)*6)+3] = (uint8_t)(0x000000FF&FrameDword[i]);
				PRFramePacket[((i+1)*6)+2] = (uint8_t)(0x000000FF&((FrameDword[i])>>8));
				PRFramePacket[((i+1)*6)+1] = (uint8_t)(0x000000FF&((FrameDword[i])>>16));
				PRFramePacket[((i+1)*6)+0] = (uint8_t)(0x000000FF&((FrameDword[i])>>24));
			}
			/*Send the data over UDP.*/
			SendPRData(clientSocket,serverAddr,PRFramePacket,((dlength*4) + 6));
		}
		/*Point to next sequence*/
	/*	if(dlength == 245) 
		{
			dlength = 1;
		}else
		{
			dlength +=1;
		}*/
		SequenceCount = SequenceCount+1;
    }

	
	/* Close bitfile after sending all packets using UDP protocol.*/
    fclose(BitFile);
	return 0;
}



int main() 
{
	configurepartialbitfile(20000,"192.168.100.10", "./bitfiles/vcu1525/PartialBlinker_i_partialblinker_partial.bin"); 
    return 0;
}
