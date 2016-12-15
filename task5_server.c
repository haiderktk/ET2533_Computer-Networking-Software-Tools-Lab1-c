/* 
 * Lab Assignment 5 TCP_server
 * reference: https://gist.github.com/silv3rm00n/5821760
 */

#include "include.h"

int i=1;
int sockets[1000];

void *multiCli_handler(void*socket_des);
int main ( int argc, char * argv[])
{
    if (argc < 2)
    {
        printf("Usage : <Port>\n");
        return 0;
    }
    if(strcmp(argv[1], "-h")==0)
    {
        printf("Help: <Port>\n");
        return 0;	
    }

	//defined variables
	int sockDes, connectSock, length;
	struct sockaddr_in serveradd, client;
	 
    //create socket
       if ((sockDes=socket(AF_INET,SOCK_STREAM,0)) < 0)
       {
		   printf("Error! Can't create socket.\n");
		   exit(0);
       }else
	   bzero(&serveradd, sizeof(serveradd));
	   serveradd.sin_family = AF_INET;
	   serveradd.sin_port = htons(atoi(argv[1]));
	   serveradd.sin_addr.s_addr = htonl(INADDR_ANY);
	  
       //bind socket
    
        if ((bind(sockDes,(struct sockaddr *)&serveradd, sizeof(serveradd))) < 0)
        {
            printf("Error! Binding socket!\n");
            exit(0);
       }
       else
       
       //Listen for connections
       if ((listen(sockDes,5)) < 0)
       {
	     printf("Error! Could not listen to connection.\n");
	     exit(0);
        }else
            {
                printf("Listening @port: [%s] \n", argv[1]);
		    }
    
		  
       //Accept
    length = sizeof(struct sockaddr_in);
    int sock_id = 0;
    while(1)
        {
            sock_id++;
            connectSock = accept(sockDes, (struct sockaddr *)&client, &length);
		
            printf("New connection (%s:%d) socket_ID %d \n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), sock_id);
            //Add new socket to chat array
            sockets[i]=connectSock;
            i++;
                
            //thread for new connection
            pthread_t thread;
            if( pthread_create( &thread , NULL ,  multiCli_handler , &connectSock) < 0)
                {
                        printf("Error! thread creation\n");
                        return 1;
                }
            pthread_detach(thread);
        }
    return 0;
  }
  // multiple client handling
void *multiCli_handler(void*socket_des)
{
	int  socket = *(int*)socket_des;
	int n, len, len_nick, len_msg, length;
    // message length 255+ nickname 12+ NICK 4+ MSG 3 = 274
	char recBuf[274], sendBuf[274], nick[20], nickname[20], msg[20], cli_msg[256], buff[274];
    char error[20] = "ERROR TEXT!\n";
        //set zero
        memset(buff,0,274);
        memset(recBuf,0,274);
        memset(sendBuf,0,274);
        memset(nick,0,6);
        memset(nickname,0,12);
        memset(msg,0,4);
        memset(cli_msg,0,256);
 
 //send hello version to client
       write(socket, "HELLO 1\n", strlen("HELLO 1\n"));
  
  //retreive nickname from client message
    while (read(socket, recBuf, 16))
       {

            strncpy(nick, &recBuf[0], 5);
            
		    if(strncmp(nick,"NICK ",5)==0)
            {
				
			   len=strlen(recBuf);
			   strncpy(nickname, &recBuf[5], len-5);
	
          //send ok to client	after catch nickname
			   write(socket,"OK!\n", strlen("OK!\n"));
	
                //read client message
                while(read(socket, recBuf, 274))
               {
				   
                   strncpy(msg, &recBuf[0], 4);

		         if(strncmp(msg,"MSG ",4)==0)
                   	 {
					   
                        len=strlen(recBuf);
                        strncpy(cli_msg, &recBuf[4], len-4);
		           
                        len_nick=strlen(nickname);
                        //nickname[len_nick -1]=' ';
                        len_msg=strlen(cli_msg);
		           
                        memset(sendBuf,0,274);
                        //The strncat function concatenates first len_nick characters from nickname to sendBuf.
                        strncpy(sendBuf, msg , 4);
                        strncat(sendBuf, nickname, len_nick);
                        strncat(sendBuf, cli_msg, len_msg);
                        length=strlen(sendBuf);                        
    			int n;  
			//send broadcast message                     
 			for(n=0;n<i;n++)
			{   	           
                   	write(sockets[n],sendBuf,length) ;
                   	}
                        memset(recBuf,0,274);
                        memset(cli_msg,0,256);
                        memset(sendBuf,0,274);
                   
                     }else
                        {
                            length=strlen(error);
                            write(socket, error, length);
                        }
               }//end while for reading
            }else
                {
                    length=strlen(error);
                    write(socket, error, length);
                }
       }//end first while for retrieve client data
printf("Client left chat.\n");
close(socket);
}


	   
