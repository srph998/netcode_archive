/*
        client.c
        FMAN-DEV
*/

#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HOST "127.0.0.1"
#define PORTNO 1337
#define MAX_LIMIT 256

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[MAX_LIMIT];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(HOST);

    if (server == NULL) {
        fprintf(stderr, "[ERROR] can't locate host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(PORTNO);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("[ERROR] on connection");
        exit(1);
    }

    while (1) {
        bzero(buffer, MAX_LIMIT);
        strncpy(buffer, "asdflasdfhkjsadhkjfaskjdfkasdfhkjasdhkjfafdkasdfksadkfh", 64);

        /* tonky tonk */

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("[ERROR] while writing to socket");
            exit(1);
        }

        break;
    }
    return 0;
}
