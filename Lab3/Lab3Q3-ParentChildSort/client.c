#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 100

int main() {
    int sockfd, retval;
    struct sockaddr_in serveraddr;
    char buffer[MAXSIZE];
    int child_received = 0;
    int parent_received = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket Creation Error");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(4444);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    retval = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        perror("Connection error");
        close(sockfd);
        return 1;
    }

    printf("Enter a string to send to the server: ");
    fgets(buffer, sizeof(buffer), stdin);

    send(sockfd, buffer, strlen(buffer), 0);

    while (!(child_received && parent_received)) {
        memset(buffer, 0, sizeof(buffer));
        recv(sockfd, buffer, sizeof(buffer), 0);

        if (strstr(buffer, "child") && !child_received) {
            printf("Received from server (PID %d):\n%s\n", getpid(), buffer);
            send(sockfd, '\0', 1, 0);
            child_received = 1;
        }

        if (strstr(buffer, "parent") && !parent_received) {
            printf("Received from server (PID %d):\n%s\n", getpid(), buffer);
            send(sockfd, '\0', 1, 0);
            parent_received = 1;
        }
    }

    close(sockfd);

    return 0;
}
