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
int
readable_or_timeout (int sd, int timeout_in_seconds)
{
  /*-----------------------------------------
    --- DATA
    -----------------------------------------
  */
  fd_set         read_sd_set;  /* set of socket descriptors that we 
			          expect to read from */
  int            status;
  struct timeval tv;           /* select() needs the timeout in this type of
				  structure */
  /*-----------------------------------------
    --- ACTION
    -----------------------------------------
  */

  /* Set the socket descriptor set to contain only the socket
     descriptor that this function was passed. */
  FD_ZERO (&read_sd_set);
  FD_SET  (sd, &read_sd_set);

  /* Convert the timeout to the timeval structure.  Select uses this
     structure because it lets you specify a timeout in microseconds
     (however, not every system will actually check down to the
     microsecond level).  */
  tv.tv_sec = 0;
  tv.tv_usec = timeout_in_seconds;  /* zero microseconds. */

  /* select will return only when I/O is pending, when it has timed
     out, or if there is an error.  Note that after this function
     returns, the descriptor sets and the timeout are UNdefined.

     The first argument of select specifies how many file/socket
     descriptors the system should check.  If the first argument is N,
     the system will check descriptors 0 through N-1.  Thus, this
     argument should be set to the highest descriptor in your sets,
     plus 1.  The reason for this argument is that a process might be
     able to handle a very high number of descriptors.  This argument
     allows the system to limit the number of descriptors it actually
     checks. */

  status = select (sd+1,         /* highest-numbered descriptor + 1 */
		   &read_sd_set, /* descriptor set for reading */
		   NULL,         /* descriptor set for writing (none) */
		   NULL,         /* descriptor set for exceptions (none) */
		   &tv);         /* time out */

  /* if there was an error or timed out, print a message. But do NOT exit. */
  if      (status <  0) {perror ("select error");}
  else if (status == 0)
    {
      printf ("timed out.  Timeout threshhold was %d secs\n",
	      timeout_in_seconds);
    }

  return (status);
}


void SendPRData(int clientsocket, struct sockaddr_in serveraddr,uint8_t *packet,int length)
{
	int rlength,readable;
	static char ibuffer[12];
	sendto(clientsocket, (const char *)packet, length,
	MSG_CONFIRM, (const struct sockaddr *) &serveraddr, 
	sizeof(serveraddr));
	readable=readable_or_timeout(clientsocket,50000);//Wait 50ms
	if (readable>0)
	{
		recvfrom(clientsocket, (char *)ibuffer,10,
			MSG_WAITALL, (struct sockaddr *) &serveraddr,&rlength);
	}
}


uint32_t configurepartialbitfile(uint32_t serverport, char *serverip,char *filename)
{
	FILE * 		BitFile;
	struct 		sockaddr_in 	serverAddr;
    ssize_t 	BytesReadSize;
	int 		clientSocket,i;
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



int main() 
{
	configurepartialbitfile(10000,"192.168.100.10", "./bitfiles/vcu118/PartialBlinker_i_partialflasher_partial.bin"); 
    return 0;
}
