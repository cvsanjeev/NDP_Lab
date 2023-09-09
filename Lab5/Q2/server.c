#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

int findAnagram(char str1[], char str2[])
{
    int count1[26] = {0}, count2[26] = {0}, i = 0;

    while (str1[i] != '\0')
    {
        count1[str1[i] - 'a']++;
        i++;
    }
    i = 0;
    while (str2[i] != '\0')
    {
        count2[str2[i] - 'a']++;
        i++;
    }
    for (i = 0; i < 26; i++)
    {
        if (count1[i] != count2[i])
            return 0;
    }
    return 1;
}

int main()
{
    int serverSocket, clientSocket, receivedBytes, sentBytes, x, numConnections = 0;
    socklen_t clientAddrLen;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[50], string1[50], string2[50];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        printf("\nSocket creation error.");
        exit(0);
    }
    printf("\nSocket created.");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4442);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bindResult = bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (bindResult == -1)
    {
        printf("\nBinding error.");
        exit(0);
    }
    printf("\nSocket binded.");

    int listenResult = listen(serverSocket, 8);
    if (listenResult == -1)
    {
        close(serverSocket);
        exit(0);
    }
    printf("\nSocket listening. Waiting for connections..");

    while (1)
    {
        clientAddrLen = sizeof(clientAddr);

        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1)
        {
            close(serverSocket);
            exit(0);
        }
        printf("\nSocket accepting.");

        receivedBytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (receivedBytes == -1)
        {
            printf("\nMessage Receiving Failed");
            close(serverSocket);
            close(clientSocket);
            exit(0);
        }

        printf("\nClient socket address received is: \nPort number: %d\n", x);
        printf("IP address is: %s\n", buffer);

        receivedBytes = recv(clientSocket, string1, sizeof(string1), 0);
        if (receivedBytes == -1)
        {
            printf("\nMessage Receiving Failed");
            close(serverSocket);
            close(clientSocket);
            exit(0);
        }
        printf("\nString 1 Received: ");
        printf("%s\n", string1);

        receivedBytes = recv(clientSocket, string2, sizeof(string2), 0);
        if (receivedBytes == -1)
        {
            printf("\nMessage Receiving Failed");
            close(serverSocket);
            close(clientSocket);
            exit(0);
        }
        printf("\nString 2 Received: ");
        printf("%s\n", string2);

        // Check for anagrams
        int isAnagram = findAnagram(string1, string2);
        if (isAnagram == 1)
            strcpy(buffer, "Strings are anagrams");
        else
            strcpy(buffer, "Strings are not anagrams");

        sentBytes = send(clientSocket, buffer, sizeof(buffer), 0);
        if (sentBytes == -1)
        {
            printf("\nMessage Sending Failed");
            close(serverSocket);
            close(clientSocket);
            exit(0);
        }

        close(clientSocket);
        numConnections++;
    }

    close(serverSocket);
    return 0;
}
