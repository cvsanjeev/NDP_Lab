#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAXSIZE 100

int compareCharsAsc(const void *a, const void *b) {
    char charA = *(char *)a;
    char charB = *(char *)b;

    return charA - charB;
}

int compareCharsDsc(const void *a, const void *b) {
    char charA = *(char *)a;
    char charB = *(char *)b;

    return charB - charA;
}

void sortString(char *str,int k) {
    size_t len = strlen(str);
    if (k==0)
        qsort(str, len, sizeof(char), compareCharsAsc);
    else
        qsort(str, len, sizeof(char), compareCharsDsc);


}
void myFunction(char *str, int k);

int main() {
    int sockfd, newsockfd, retval;
    socklen_t actuallen;
    struct sockaddr_in serveraddr, clientaddr;
    char buffer[MAXSIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation error");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(4444);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    retval = bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    if (retval == -1) {
        printf("Binding error");
        close(sockfd);
        exit(1);
    }

    retval = listen(sockfd, 1);
    if (retval == -1) {
        printf("Listening error");
        close(sockfd);
        exit(1);
    }

    printf("Server Is Running\n");

    actuallen = sizeof(clientaddr);
    newsockfd = accept(sockfd, (struct sockaddr *)&clientaddr, &actuallen);
    if (newsockfd == -1) {
        printf("Accept error");
        close(sockfd);
        exit(1);
    }

    // Main Program Starts Here

    recv(newsockfd, buffer, sizeof(buffer), 0);
    printf("%s",buffer);

    if (fork() == 0) { // Child Process
        printf("Child process (PID %d) is running.\n", getpid());

        myFunction(buffer, 0);
        send(newsockfd, buffer, strlen(buffer), 0);
        printf("Child process (PID %d) has sorted numbers and sent the result ||%s|| to the client.\n", getpid(), buffer);

        char ack;
        recv(newsockfd, ack, sizeof(ack), 0);
        printf("Parent process (PID %d) received acknowledgment from the client.\n", getpid());
        exit(1);

    } else { // Parent Process
        printf("Parent process (PID %d) is running.\n", getpid());

        myFunction(buffer, 1);
        send(newsockfd, buffer, strlen(buffer), 0);
        printf("Parent process (PID %d) has sorted numbers and sent the result ||%s|| to the client.\n", getpid(), buffer);

        char ack;
        recv(newsockfd, ack, sizeof(ack), 0);
        printf("Parent process (PID %d) received acknowledgment from the client.\n", getpid());

    }

    close(newsockfd);
    close(sockfd);

    return 0;
}

void myFunction(char *str, int k) {

    printf("\nGOT STRING%s||",str);



    if (k == 0){
        sortString(str,0);
        printf("\nASC STRING%s||",str);
        strcat(str, "\n(^^from child)");
    }
    else{
        sortString(str,1);
        printf("\nDSC STRING%s||",str);
        strcat(str, "(^^from parent)");
    }

}
