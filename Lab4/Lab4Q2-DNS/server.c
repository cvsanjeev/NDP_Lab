// Server side program that sends
// a 'hi client' message
// to every client concurrently

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

// char output[100];

int search(const char* buffer, char* output) {
    FILE *fptr = fopen("domains.txt", "r");
    if (fptr == NULL) {
        perror("Error opening file");
        return 0;
    }

    char readbuffer[50];

    while (fscanf(fptr, "%s", readbuffer) == 1) {

		printf("%s",readbuffer);
        if (strcmp("endoffile", readbuffer) == 0) {
            strcpy(output, "unknown");
            fclose(fptr);
            return 1;
        }

        char temp[50];
        if (strcmp(buffer, strtok(readbuffer, ",")) == 0) {
            strcpy(output, strtok(NULL, ","));
            fclose(fptr);
            return 1;
        }
    }

    fclose(fptr);
    return 0;
}

int main()
{

	struct sockaddr_in serverAddr;                              // Server socket address structures
	int clientSocket;                                           // Client socket id
	struct sockaddr_in cliAddr;                                 // Client socket address structures
	socklen_t addr_size;                                        // Stores byte size of server socket address

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);                   // Creates a TCP socket id from IPV4 family
	printf("Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));              // Initializing address structure with NULL
	serverAddr.sin_family = AF_INET;                            // Assign port number and IP address to the socket created
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr= inet_addr("127.0.0.1");         // 127.0.0.1 is a loopback address

	bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)); // Binding the socket id with the socket structure
	listen(sockfd, 0);


	while (1)
	{
		clientSocket = accept(sockfd, (struct sockaddr*)&cliAddr,&addr_size);  // Accept clients and store their information in cliAddr
		printf("Connection accepted from %s:%d\n",inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));  // Displaying information of connected client

		if(fork()==0){

			while(1){
			char buffer[100];
			char output[100];
			bzero(buffer, sizeof(buffer));
			recv(clientSocket, buffer, 100, 0);
			if (buffer[0]=='\0')
				continue;
			printf("RECEIVED REQUEST FOR\t");
			puts(buffer);
			if(strcmp(buffer,"exit")==0)
				exit(1);

			search(buffer,output);
			puts(output);
			send(clientSocket,output,sizeof(output),0);
			}
		}

		puts("Fork created for this client. Listening for other Clients\n");

	}




	close(clientSocket);                                     // Close the client socket id
	return 0;
}

