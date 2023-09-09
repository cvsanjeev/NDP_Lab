#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4448
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Connection failed");
        exit(0);
    }

    char filename[MAXLINE];
    char option[MAXLINE];
    char search_string[MAXLINE];
    char replace_string[MAXLINE];

    printf("Enter the filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strlen(filename) - 1] = '\0';  // Remove newline character

    // Send the filename to the server
    send(sockfd, filename, sizeof(filename), 0);
    memset(buffer,0,sizeof(buffer));
    recv(sockfd, buffer, sizeof(buffer),0);
    printf("%s",buffer);
    if(strstr(buffer,"not found"))
        exit(0);



    fgets(buffer,sizeof(buffer),stdin);
    send(sockfd, buffer, sizeof(buffer), 0);
    if(strstr(buffer,"1")){
        printf("Detected Option1");
        memset(buffer,0,sizeof(buffer));
        fgets(buffer,sizeof(buffer),stdin);
        send(sockfd, buffer, sizeof(buffer), 0);
    }
    if(strstr(buffer,"2")){
        printf("Detected Option2");
        memset(buffer,0,sizeof(buffer));
        fgets(buffer,sizeof(buffer),stdin);
        send(sockfd, buffer, sizeof(buffer), 0);
        memset(buffer,0,sizeof(buffer));
        fgets(buffer,sizeof(buffer),stdin);
        send(sockfd, buffer, sizeof(buffer), 0);
    }


    recv(sockfd, buffer, sizeof(buffer),0);




    close(sockfd);

    return 0;
}
