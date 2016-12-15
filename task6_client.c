/* 
 * Lab Assignment 6 UDP SNTP client
 * Server IP address: 206.246.122.250 Port No. 123
 */

#include "include.h"
int main(int argc, char *argv[]) 
{

	if (argc < 3)
	{
		printf ("Usage : <Server IP Address> <Port Number> \n");
		return 0;
	}
	if(strcmp(argv[1], "-h")==0)
    	{
        	printf("Help: <IP Address> <Port Number>\n");
        	return 0;	
    	}

   	char *hostN=argv[1];
   	int port=atoi(argv[2]);		                        
    	int len=1024;	                       			
    	unsigned char message[48]={0b00100011,0,0,0,0,0,0,0,0};	
    	int  buffer[len];                
    struct protoent *protocol;		
    struct sockaddr_in serveradd;
    int	sockDes;	
    unsigned long transmited_time;	

    protocol=getprotobyname("udp");
    sockDes=socket(AF_INET, SOCK_DGRAM, protocol->p_proto);
    
    serveradd.sin_family      = AF_INET;
    serveradd.sin_addr.s_addr = inet_addr(hostN);
    serveradd.sin_port       = htons(port);

   // send Message to the server
	if(sendto(sockDes,message,sizeof(message),0,(struct sockaddr *)&serveradd,sizeof(serveradd))<0)
	{
		printf("Error! Can not Send message.\n");
		   exit(0);
	}
	//receive message from server
	if(recv(sockDes,buffer,len,0)<0)
	{
		printf("Error! Can not Recieve.\n");
		   exit(0);
	}

    transmited_time=ntohl((time_t)buffer[10]);	
    printf("Transmitted time is %lu\n",transmited_time);
close(sockDes);
return 0;
}
