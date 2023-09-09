 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>

// Function to reverse a string


int isPalindrome(const char *str,int len);
void countVowels(const char *str, int *vowelCounts);
void myFunction(char *str);

int main() {


    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12346);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    printf("Waiting for a connection...\n");
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);


    printf("Connected to client\n");

    char buffer[1024];
    while (1) {
        memset(&buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);
        myFunction(buffer);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    close(server_socket);

    return 0;
}



int isPalindrome(const char *str,int len) {
    int left = 0;
    int right = len - 1;

    while (left < right) {
        if (str[left] != str[right]) {
            return 0; // Not a palindrome
        }
        left++;
        right--;
    }

    return 1; // Palindrome
}

void countVowels(const char *str, int *vowelCounts) {
    for (int i = 0; str[i] != '\0'; i++) {
        char ch = tolower(str[i]); // Convert to lowercase for case-insensitive comparison
        if(ch=='a') vowelCounts[0]++;
        if(ch=='e') vowelCounts[1]++;
        if(ch=='i') vowelCounts[2]++;
        if(ch=='o') vowelCounts[3]++;
        if(ch=='u') vowelCounts[4]++;
    }
}

void myFunction(char *str){


    int len=-1;
    int vowelCounts[5] = {0};
    char temp[256];

    for(int i=0;str[i]!='\0';i++)
        len++;


    if (isPalindrome(str,len)) {
            countVowels(str, vowelCounts);

            sprintf(temp, "Length of the string is %d\nThe string is a palindrome.\nA: %d\nE: %d\nI: %d\nO: %d\nU: %d\n",len,vowelCounts[0],vowelCounts[1],vowelCounts[2],vowelCounts[3],vowelCounts[4]);

    } else {
        sprintf(temp, "Length of the string is %d\nThe string is NOT a palindrome.",len);

    }
    strcpy(str,temp);

}
