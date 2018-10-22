#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
#define PORT     10000

/*#define _USE_JUMBO_FRAMES_ 1 */

#define MAXLINE 4

#if defined(_USE_JUMBO_FRAMES_)
/* For jumbo frames use ethernet MTU of 9600 bytes*/
#define MAXSEND 9600-64
#else
/* For normal ethernet MTU we use 1500 bytes*/
#define MAXSEND 1500-64
#endif


int main() 
{
    int sockfd;
    char buffer[MAXSEND+64];
    int n, len;
    int sendnum=MAXSEND;
    int iterations;
    struct sockaddr_in     servaddr;
 
    /* Create and open a network socket file descriptor */
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
 
    memset(&servaddr, 0, sizeof(servaddr));
     
    /* UDP Application server information
     * implemented on FPGA fabric. */
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("192.168.100.10");
    /* Send packets from the smallest frame size to the maximum that the FPGA can handle.*/ 
    for (n = 0;n < MAXSEND; n++) buffer[n]=' '+(n%2);
	for(iterations = 0; iterations <MAXLINE;iterations++)
	{
		for(sendnum = 1;sendnum<MAXSEND;sendnum++)
		{
		    sendto(sockfd, (const char *)buffer, sendnum,
			MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
			    sizeof(servaddr));
			 
		    n = recvfrom(sockfd, (char *)buffer, MAXSEND, 
				MSG_WAITALL, (struct sockaddr *) &servaddr,
				&len);
		}
	}
 
    close(sockfd);
    return 0;
}
