#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXSIZE 90

// PORT number
#define PORT 4444

int main()
{
	int clientSocket, ret;                                                           // Socket id
	struct sockaddr_in cliAddr, serverAddr;                                          // Client socket structure

	clientSocket = socket(AF_INET,SOCK_STREAM, 0);                                   // Creating socket id
	if (clientSocket < 0)	exit(1);

	printf("Client Socket is created.\n");

	memset(&cliAddr, '\0', sizeof(cliAddr));                                        // Initializing socket structure with NULL

	serverAddr.sin_family = AF_INET;                                                // Assigning port number and IP address
	serverAddr.sin_port = htons(PORT);

	serverAddr.sin_addr.s_addr= inet_addr("127.0.0.1");                             // 127.0.0.1 is Loopback IP
	ret = connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));   // connect() to connect to the server

	if (ret < 0) {
		printf("Error in connection.\n");
		exit(1);
	}

	printf("Connected to Server.\n");
	char sbuff[MAXSIZE];
	char rbuff[MAXSIZE];
	int recedbytes,sentbytes,flag=0;
	pid_t childpid;

	childpid = fork();
	while (1)
	{

		if(childpid==0)
		{
			bzero(rbuff, sizeof(rbuff));
			recedbytes=recv(clientSocket,rbuff,sizeof(rbuff),0);
			if(rbuff[0]=='e' && rbuff[1]=='x' && rbuff[2]=='i' && rbuff[3]=='t') 
				{
					printf("exit"); 		
					send(clientSocket,rbuff,sizeof(rbuff),0);
					exit(1);
				}
				
			printf("recvd with PID%d and PPID%d\n",getpid(),getppid());	
			printf(">>%s \n",rbuff);
		}


		else
		{

			memset(sbuff, '\0', sizeof(sbuff));
			
			if(flag==1) {
					printf("exit"); 		
					sbuff[0]=='e';
					sbuff[1]=='x';
					sbuff[2]=='i';
					sbuff[3]=='t';
					send(clientSocket,sbuff,sizeof(sbuff),0);
					exit(1);
				}
				
			gets(sbuff);
			sentbytes=send(clientSocket,sbuff,sizeof(sbuff),0);
			printf("sent with PID%d and PPID%d\n",getpid(),getppid());
			if(sentbytes==-1)	{close(clientSocket);}

		}


	}



	return 0;
}
