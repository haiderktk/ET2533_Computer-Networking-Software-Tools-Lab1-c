/* 
 * Lab Assignment 5 TCP_client
 * reference http://www.tenouk.com/Module40c.html
 */

#include "include.h"
int main ( int argc, char * argv[])
{
    if (argc < 4)
    {
        printf("Usage : <IP> <Port> <Nick>\n");
        return 0;
    }
    if(strcmp(argv[1], "-h")==0)
    {
        printf("Help: <IP Address> <Port Number> <Nick Name>\n");
        return 0;	
    }

	//defined variables
	int n, length, sockDes;
	struct sockaddr_in serveradd;
    struct hostent *host;
	char message[256], sendBuf[276], recBuf[276], nickName[20], ack[20];
	char *nickN;
	pid_t procID;//process ID
    	nickN=argv[3];
    
    if ((length = strlen(nickN)) >12)
    {
        printf("Error!Nick Name is more than 12 characters.\n");
        exit(0);
    }else
        {
            nickN[length]=' ';
            nickN[length+1]='\0';
        }
    // get the host info
   if((host=gethostbyname(argv[1])) == NULL)
    {
        perror("gethostbyname()");
        exit(0);
    }
    //socket creation
       if ((sockDes=socket(AF_INET,SOCK_STREAM,0)) < 0)
       {
		   printf("Error! Can not create a socket.\n");
		   exit(0);
       }else
	   memset(&serveradd, 0, sizeof(serveradd));
	   
	   serveradd.sin_family = AF_INET;
	   serveradd.sin_port = htons(atoi(argv[2]));
	   
        serveradd.sin_addr= *((struct in_addr *)host->h_addr);
            if (connect(sockDes, (struct sockaddr *)&serveradd, sizeof(struct sockaddr))<0)
            {
                printf("Error! Can not connect to the server.\n");
                exit(0);
            }else
                {
                    printf("connect to the server.\n");
                    n=read(sockDes, ack, 50);
                    ack[n] = '\0';
                    printf("%s", ack);
                    if(strncmp(ack,"HELLO 1\n", 7)==0)
                    {
                        strcpy(nickName, "NICK ");
                        strcat(nickName, nickN);
                        length = strlen(nickName);
                        write(sockDes,nickName,length);
                        n = read(sockDes, ack, 50);
                        ack[n] = '\0';
                        printf("%s", ack);
                        }else
                            {
                                printf("Error!Does Not receive Ack from server.\n");
                                exit(0);
                            }
		   
        //fork for duplex communication
      procID=fork();
       
       if(procID == -1){
		   printf("Error!Can NOT fork");
		   exit(0);
		   }
		   else if (procID == 0)
			{
			   
			  while(n = read(sockDes, recBuf, 276))
			   {
			   recBuf[n] = '\0';
		           printf("%s", recBuf);
		   	   }
		          exit(0);
		   	 }
		   else {
                        while(fgets(message, 256, stdin))
                        {
                                length = strlen(message);
                                if (length <= 255)
                                {
                                    strcpy(sendBuf, "MSG ");
                                    strcat(sendBuf,message);
                                    length = strlen(sendBuf);
                                    write(sockDes,sendBuf, length);
                                }else
                                    {
                                        printf("Error!Message is limited to 255 characters.\n");
                                    }

                            }
                    exit(0);
			   
                    }
}
close(sockDes);
return(0);
}
	   
