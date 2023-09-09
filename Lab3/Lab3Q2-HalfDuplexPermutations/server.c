#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 100

void calculatePermutations(char *str, int l, int r, char *permutations, int *index);

int main() {
    int sockfd, retval;
    socklen_t addrlen;
    struct sockaddr_in serveraddr, clientaddr;
    char buffer[MAXSIZE];
    char permutations[MAXSIZE * MAXSIZE]; // Array to store all permutations
    int index = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Socket creation error");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(3388);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Binding error");
        close(sockfd);
        exit(1);
    }

    printf("Server Is Running\n");

    addrlen = sizeof(clientaddr);
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, &addrlen);

    printf("Received from client: %s\n", buffer);

    calculatePermutations(buffer, 0, strlen(buffer) - 1, permutations, &index);

    // After storing all permutations, send the entire array to the client
    sendto(sockfd, permutations, strlen(permutations), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));

    close(sockfd);

    return 0;
}

void swap(char *x, char *y) {
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void calculatePermutations(char *str, int l, int r, char *permutations, int *index) {
    if (l == r) {
        // Calculate and store each permutation
        strncpy(permutations + (*index), str, r + 1);
        *index += r + 1;
    } else {
        for (int i = l; i <= r; i++) {
            swap((str + l), (str + i));
            calculatePermutations(str, l + 1, r, permutations, index);
            swap((str + l), (str + i)); // backtrack
        }
    }
}
