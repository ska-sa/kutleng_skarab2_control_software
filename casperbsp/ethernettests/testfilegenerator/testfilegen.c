#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main() 
{
	FILE * 		BitFile;
    BitFile = fopen("testfile.bin","w+b");
    unsigned char Data[4];  
    /* Check if and error occured while opening the bitfile.*/
    if (BitFile == NULL)
    {
		exit(EXIT_FAILURE);
	}
	
	fseek(BitFile,0L,SEEK_SET);
	Data[0]=1;
	Data[1]=2;
	Data[2]=3;
	Data[3]=4;
	
	for (int i=0;i<9000;i++)
	{
		fwrite(&Data[0],1,4,BitFile);
		Data[0]=Data[0]+4;
		Data[1]=Data[1]+4;
		Data[2]=Data[2]+4;
		Data[3]=Data[3]+4;
	}
    fclose(BitFile);
    return 0;
}
