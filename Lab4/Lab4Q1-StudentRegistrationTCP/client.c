#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 4444

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    int option;
    char response[100];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        printf("Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error in connection");
        exit(1);
    }

    printf("Enter option (1 for Registration Number, 2 for Student Name, 3 for Subject Code): ");
    scanf("%d", &option);

    // Send the selected option to the server
    send(client_fd, &option, sizeof(int), 0);

    if (option == 2 || option == 3) {
        printf("Enter search string: ");
		scanf("%s",response);
        // Send the search string to the server
        send(client_fd, response, sizeof(response), 0);

    }

    else if(option == 1 ){
		printf("Enter search number: ");
		scanf("%d", &option);

        // Send the search string to the server
        send(client_fd, &option, sizeof(int), 0);

        // Receive and display the response from the server



	}

    else {
        printf("Invalid option\n");
    }
        recv(client_fd, response, sizeof(response), 0);
        printf("Response from Server:\n%s\n", response);

    close(client_fd);

    return 0;
}
