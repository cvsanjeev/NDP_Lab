#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXSIZE 100

int main() {
    int sockfd, retval;
    struct sockaddr_in serveraddr;
    char buffer[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Socket Creation Error");
        return 1;
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter a string: ");
    fgets(buffer, sizeof(buffer), stdin);

    sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    printf("Receiving permutations:\n");

    while (1) {
        retval = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        if (retval <= 0) {
            break;
        }
        if (strcmp(buffer, "DONESENDING") == 0) {
            printf("Server has finished sending permutations.\n");
            break;
        } else {
            printf("%s\n", buffer);
        }
    }

    close(sockfd);

    return 0;
}
