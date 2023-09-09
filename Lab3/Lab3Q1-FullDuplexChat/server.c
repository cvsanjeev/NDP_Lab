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

	int sockfd, ret;                                            // Server socket id
	struct sockaddr_in serverAddr;                              // Server socket address structures
	int clientSocket;                                           // Client socket id

	struct sockaddr_in cliAddr;                                 // Client socket address structures

	socklen_t addr_size;                                        // Stores byte size of server socket address

	pid_t childpid;                                             // Child process id

	sockfd = socket(AF_INET, SOCK_STREAM, 0);                   // Creates a TCP socket id from IPV4 family

	if (sockfd < 0) {                                           // Error handling if socket id is not valid
		printf("Error in connection.\n");
		exit(1);
	}

	printf("Server Socket is created.\n");
	memset(&serverAddr, '\0', sizeof(serverAddr));              // Initializing address structure with NULL
	serverAddr.sin_family = AF_INET;                            // Assign port number and IP address to the socket created
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr= inet_addr("127.0.0.1");         // 127.0.0.1 is a loopback address

	ret = bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)); // Binding the socket id with the socket structure

	if (ret < 0)
	{                                              // Error handling
		printf("Error in binding.\n");
		exit(1);
	}

	listen(sockfd,0);

	clientSocket = accept(sockfd, (struct sockaddr*)&cliAddr,&addr_size);  // Accept clients and store their information in cliAddr
		if (clientSocket < 0)                              // Error handling
		{
			exit(1);
		}
	childpid = fork();
	printf("Connection accepted from %s:%d\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port) );  // Displaying information of connected client
	printf("Client connected\n\n");        // Print number of clients connected till now



	char sbuff[MAXSIZE];
	char rbuff[MAXSIZE];
	int recedbytes,sentbytes,flag=0;
	char opt,mode[5];
	memset(rbuff, '\0', sizeof(rbuff));
	
	
	while (1)
	{


		//RECEIVER
		if ( childpid == 0 )
		{
			
			memset(rbuff, '\0', sizeof(rbuff));
			recedbytes=recv(clientSocket,rbuff,sizeof(rbuff),0);
			if(rbuff[0]=='e' && rbuff[1]=='x' && rbuff[2]=='i' && rbuff[3]=='t') 
				{
					printf("exit"); 		
					send(clientSocket,rbuff,sizeof(rbuff),0);
					printf("sent with PID%d and PPID%d",getpid(),getppid());
					exit(1);
				}

			printf("recvd with PID%d and PPID%d\n",getpid(),getppid());
			printf(">>%s \n",rbuff);

		}



		//SENDER
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
					break;
				}
			
			gets(sbuff);
			sentbytes=send(clientSocket,sbuff,sizeof(sbuff),0);
			printf("sent with PID%d and PPID%d\n",getpid(),getppid());
			if(sentbytes==-1)	{close(sockfd);close(clientSocket);}


		}







	}
	close(sockfd);                            // Closing the server socket id
	close(clientSocket);                                     // Close the client socket id
	return 0;
}
