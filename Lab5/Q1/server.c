#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXLINE 4096 /* max text line length */
#define SERV_PORT 3000 /* port */
#define LISTENQ 8 /* maximum number of client connections */

int main() {
    int server_socket, client_socket, recv_bytes, send_bytes, client_count = 0;
    socklen_t len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[100];
    char str[100];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("\nSocket creation error.");
        exit(0);
    }
    printf("\nSocket created.");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bind_result = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_result == -1) {
        printf("\nBinding error.");
        exit(0);
    }
    printf("\nSocket binded.");

    int listen_result = listen(server_socket, LISTENQ);
    if (listen_result == -1) {
        close(server_socket);
        exit(0);
    }
    printf("\nSocket listening.\n");

    int childpid, n;
    FILE *file_ptr;

    for (;;) {
        len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &len);
        if (client_socket == -1) {
            close(server_socket);
            exit(0);
        }
        printf("\nSocket accepting.\n");

        if (client_count == 2) {
            printf("\nMore than 2 clients!\n");
            close(client_socket);
            break;
        }
        client_count++;

        if ((childpid = fork()) == 0) { // if it’s 0, it’s a child process
            close(server_socket);

            n = recv(client_socket, buffer, sizeof(buffer), 0);
            if (n < 0) {
                printf("%s\n", "Read error");
                exit(0);
            }

            file_ptr = fopen("tmp.txt", "a");
            fputs(buffer, file_ptr);
            fclose(file_ptr);

            char buffer2[50];
            n = recv(client_socket, buffer2, sizeof(buffer2), 0);
            if (n < 0) {
                printf("%s\n", "Read error");
                exit(0);
            }

            file_ptr = fopen("tmp2.txt", "a");
            fputs(buffer2, file_ptr);
            fclose(file_ptr);

            if (client_count == 2) {
                FILE *fp;
                int line_num = 1;
                char temp[512];

                if ((fp = fopen("tmp.txt", "r")) == NULL) {
                    close(server_socket);
                    exit(0);
                }

                while (fgets(temp, 512, fp) != NULL) {
                    strcpy(str, temp);
                    printf("%s", str);
                    line_num++;
                }

                if (fp) {
                    fclose(fp);
                }

                FILE *fp2;
                int line_num2 = 1;
                char temp2[512];

                if ((fp2 = fopen("tmp2.txt", "r")) == NULL) {
                    close(server_socket);
                    exit(0);
                }

                while (fgets(temp2, 512, fp2) != NULL) {
                    strcpy(str, temp2);
                    printf("\n%s", str);
                    line_num2++;
                }

                if (fp2) {
                    fclose(fp2);
                }
            }
            exit(0);
        }
        // close client socket
        close(client_socket);
    }
    return 0;
}
