#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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


int main(){

    char A[4];
    A[0]='D';
    A[1]='B';
    A[2]='C';
    sortString(A,0);
    printf("%s",A);


}


