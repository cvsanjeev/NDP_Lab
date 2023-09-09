#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4448
#define MAXLINE 1024
void fun3(int newsockfd,char *fp);
void fun2(int newsockfd, char* str1, char* str2, char *fp);
int fun1(int newsockfd, char* buffer, const char *fp);
int search_string_count(char* file_content, char* search_string);
int compareCharsAsc(const void *a, const void *b) {    char charA = *(char *)a;    char charB = *(char *)b;    return charA - charB;    }
void sortString(char *str) {    size_t len = strlen(str); qsort(str, len, sizeof(char), compareCharsAsc); }


int main() {
    int sockfd, newsockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind socket
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("Socket bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sockfd, 5) != 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    len = sizeof(cliaddr);
    newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &len);

    // File handling
    FILE* file;
    char filename[MAXLINE];
    char file_content[MAXLINE];

    memset(filename, 0, sizeof(filename));
    memset(file_content, 0, sizeof(file_content));

    recv(newsockfd, filename, sizeof(filename), 0);

    // Try to open the file
    file = fopen(filename, "r");
    if (file == NULL) {
        send(newsockfd, "File not found", sizeof("File not found"), 0);
        close(newsockfd);
        close(sockfd);
        exit(0);
    }

    else
        while (fgets(buffer, sizeof(buffer), file) != NULL)
            strcat(file_content,buffer);

    printf("%s",file_content);

    //FILESTREAM FUNCTIONS
    send(newsockfd, "1. Search 2. Replace 3. Reorder 4. Exit", sizeof("1. Search 2. Replace 3. Reorder 4. Exit"), 0);

    memset(buffer,0,sizeof(buffer));
    recv(newsockfd, buffer, sizeof(buffer),0);
    printf("%s",buffer);

    if(strstr(buffer,"1"))
        {
            memset(buffer,0,sizeof(buffer));
            recv(newsockfd, buffer, sizeof(buffer),0);
            int temp = fun1(newsockfd,file_content,buffer);
            sprintf(buffer,"THE SEARCH KEY OCCORS%d TIMES",temp);
            send(newsockfd, buffer, sizeof(buffer), 0);

        }

    else if(strstr(buffer,"2"))
        {
            char str1[100],str2[100];
            memset(str1,0,sizeof(str1));
            recv(newsockfd, str1, sizeof(str1),0);
            memset(str2,0,sizeof(str2));
            recv(newsockfd, str2, sizeof(str2),0);
            fun2(newsockfd,str1,str2,file_content);
        }

    else if(strstr(buffer,"3"))
        fun3(newsockfd,file_content);

    else
        exit(0);




    fclose(file);
    close(newsockfd);
    close(sockfd);

    return 0;
}




int search_string_count(char* file_content, char* search_string) {
    int count = 0;
    char* pos = file_content;

    while ((pos = strstr(pos, search_string)) != NULL) {
        count++;
        pos += strlen(search_string);
    }

    return count;
}

int fun1(int newsockfd, char *str1, const char *str2) {

    int count = 0;
    const char *ptr = str1;

    if (str2[0] == '\0') {
        // Return 0 if str2 is an empty string (no occurrences).
        return 0;
    }

    while ((ptr = strstr(ptr, str2)) != NULL) {
        count++;
        ptr++;  // Move the pointer to check for the next occurrence.
    }

    return count;
}

void fun2(int newsockfd,char* str1,char* str2, char* file_content) {


    char* pos = strstr(file_content, str1);

    if (pos != NULL) {
        strcpy(pos, str2);
        send(newsockfd, "String replaced", sizeof("String replaced"), 0);
    } else {
        send(newsockfd, "String not found", sizeof("String not found"), 0);
    }
}

void fun3(int newsockfd, char* file_content) {

    sortString(file_content);
    printf("%s",file_content);
    send(newsockfd,"Sorted end the conn",sizeof("Sorted end the conn"),0);

}


