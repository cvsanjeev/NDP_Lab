#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netdb.h>
#include <errno.h>

int main() {
    int socket_fd, connection_result, send_bytes;
    struct sockaddr_in server;
    char message[50];

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        printf("\nSocket creation error.");
        exit(0);
    }
    printf("\nSocket created.");

    server.sin_family = AF_INET;
    server.sin_port = htons(4442);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connection_result = connect(socket_fd, (struct sockaddr*)&server, sizeof(server));
    if (connection_result == -1) {
        printf("\nConnection error.");
        exit(0);
    }
    printf("\nSocket connected.");

    strcpy(message, "Institute Of");

    send_bytes = send(socket_fd, message, sizeof(message), 0);
    if (send_bytes == -1) {
        close(socket_fd);
        printf("\nMessage Sending Failed");
        exit(0);
    }

    struct hostent *host_entry;
    int hostname;
    char host_buffer[256];
    char *ip_buffer;
    hostname = gethostname(host_buffer, sizeof(host_buffer));
    host_entry = gethostbyname(host_buffer);
    ip_buffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
    char ip[50];
    strcpy(ip, ip_buffer);
    strcat(ip, "    ");
    printf("\nIP is: %s", ip);

    send_bytes = send(socket_fd, ip, sizeof(ip), 0);
    if (send_bytes == -1) {
        close(socket_fd);
        printf("\nMessage Sending Failed");
        exit(0);
    }

    close(socket_fd);
}
