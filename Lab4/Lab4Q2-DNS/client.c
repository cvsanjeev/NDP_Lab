// Client Side program to test
// the TCP server that returns
// a 'hi client' message

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// PORT number
#define PORT 4449


int main()
{
	int clientSocket, ret;                                                           // Socket id
	struct sockaddr_in cliAddr, serverAddr;                                          // Client socket structure
	char buffer[1024];                                                               // char array to store incoming message

	clientSocket = socket(AF_INET,SOCK_STREAM, 0);                                   // Creating socket id
	printf("Client Socket is created.\n");

	memset(&cliAddr, '\0', sizeof(cliAddr));                                        // Initializing socket structure with NULL
	serverAddr.sin_family = AF_INET;                                                // Assigning port number and IP address
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr= inet_addr("127.0.0.1");                             // 127.0.0.1 is Loopback IP

	connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));   // connect() to connect to the server
	printf("Connected to Server.\n");



	while (1)
	{
		printf("Enter a domain name to lookup\nEnter 'exit' to exit\n");
		bzero(buffer, sizeof(buffer));
		fgets(buffer,1023,stdin);
		send(clientSocket,buffer,sizeof(buffer),0);
		if(strcmp(buffer,"exit")==0)
			return 0;


		bzero(buffer, sizeof(buffer));
		recv(clientSocket,buffer, 100, 0);
		puts(buffer);

	}



	return 0;
}

