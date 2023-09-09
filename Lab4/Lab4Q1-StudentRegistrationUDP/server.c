#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 4445

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    int option;
    char response[100];

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        printf("Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    printf("Enter option (1 for Registration Number): ");
    scanf("%d", &option);

    sendto(client_socket, &option, sizeof(int), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (option == 1) {
        int regNumber;
        printf("Enter registration number: ");
        scanf("%d", &regNumber);
        sendto(client_socket, &regNumber, sizeof(int), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        socklen_t addr_size = sizeof(server_addr);
        recvfrom(client_socket, response, sizeof(response), 0, (struct sockaddr*)&server_addr, &addr_size);
        printf("Response from Server:\n%s\n", response);
    } else {
        printf("Invalid option\n");
    }

    close(client_socket);

    return 0;
}
