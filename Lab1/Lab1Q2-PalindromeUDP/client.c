 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>



int main() {

    int client_socket;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12346);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    char buffer[1024];

    while (1) {

        fgets(buffer, sizeof(buffer), stdin);

        if (strcmp(buffer, "exit") == 0)
            break;

        else
            send(client_socket, buffer, strlen(buffer), 0);

        memset(&buffer,0,sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);

        printf("Response: %s\n", buffer);
    }

    close(client_socket);

    return 0;
}



