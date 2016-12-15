/* 
 * Lab Assignment 6 UDP SNTP server 
 */

#include "include.h"
int main(int argc, char *argv[]) 
{

	if (argc < 2)
	{
		printf ("Usage : <Port Number> \n");
		return 0;
	}
	if(strcmp(argv[1], "-h")==0)
    	{
        	printf("Help: <Port Number>\n");
        	return 0;	
    	} 
struct sockaddr_in serveradd, client;
//struct sockaddr_in cli;
int len = sizeof(struct sockaddr_in);
int sockDes;

long int buffer[12];

//create socket
       if ((sockDes=socket(AF_INET,SOCK_DGRAM,0)) < 0)
       {
		   printf("Error! Can't create socket.\n");
		   exit(0);
       }else
	   bzero(&serveradd, sizeof(serveradd));
	   serveradd.sin_family = AF_INET;
	   serveradd.sin_port = htons(atoi(argv[1]));
	   serveradd.sin_addr.s_addr = htonl(INADDR_ANY);
 	if ((bind(sockDes,(struct sockaddr *)&serveradd, sizeof(serveradd))) < 0)
        {
            printf("Error! Binding socket!\n");
            exit(0);
        }
       
	printf("Time Server is waiting clients to connect...\n");

while(1) 
{
	if(recvfrom(sockDes,buffer,sizeof(buffer),0,(struct sockaddr*)&client,&len)<0)
	{
		printf("Error! Could not Recieve.\n");
		exit(0);	
	}
//datalen=recvfrom(ser,buffer,sizeof(buffer),0,(struct sockaddr*)&cli,&sockaddrlen);
    time_t tim;
    
unsigned long int  z = 2208988800u;
unsigned long int sina;
long int buff[12];
sina=htonl(610100e3);
// unicast message header set to zero except first octet.LV=0,version=4,mode=4(server)
buff[0] =sina;
buff[1] = 0;
buff[2] = 0;
buff[3] = 0;
buff[4] = buffer[4];
buff[5] = 0;
buff[6] = 0;
buff[7] = 0;
buff[8] = 0;
buff[9] = 0;
buff[10] = htonl((unsigned long int)time(&tim) + z);;
buff[11] = 0;

	if((sendto(sockDes,buff,sizeof(buff),0,(struct sockaddr*)&client,len))<0)
	{
		printf("Error!Could not Send.\n");
		exit(0);
	}
}
close(sockDes);
return 0;
}		
