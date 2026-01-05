/*

*/

#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PORTNO 1337
#define MAX_LIMIT 256

void bzero(void *a, size_t n) { memset(a, 0, n); }

void bcopy(const void *src, void *dest, size_t n) { memmove(dest, src, n); }

struct sockaddr_in *init_sockaddr_in(uint16_t port_number) {
        struct sockaddr_in *socket_address = malloc(sizeof(struct sockaddr_in));
        memset(socket_address, 0, sizeof(*socket_address));
        socket_address->sin_family = AF_INET;
        socket_address->sin_addr.s_addr = htonl(INADDR_ANY);
        socket_address->sin_port = htons(port_number);
        return socket_address;
}

char *process_operation(char *input) {
        size_t n = strlen(input) * sizeof(char);
        char *output = malloc(n);
        memcpy(output, input, n);
        return output;
}

int main(int argc, char *argv[]) {
        const uint16_t port_number = PORTNO;
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in *server_sockaddr = init_sockaddr_in(port_number);
        struct sockaddr_in *client_sockaddr = malloc(sizeof(struct sockaddr_in));
        socklen_t server_socklen = sizeof(*server_sockaddr);
        socklen_t client_socklen = sizeof(*client_sockaddr);

        if (bind(server_fd, (const struct sockaddr *)server_sockaddr, server_socklen) < 0) {
                printf("[ERROR] Bind Error\n");
                exit(0);
        }

        if (listen(server_fd, 3) < 0) {
                printf("[ERROR] Listen Error\n");
                exit(0);
        }

        const size_t buffer_len = MAX_LIMIT;
        char *buffer = malloc(buffer_len * sizeof(char));
        char *response = NULL;
        time_t last_operation;
        __pid_t pid = -1;

        // loop
        while (1) {
                int client_fd = accept(server_fd, (struct sockaddr *)&client_sockaddr, &client_socklen);

                pid = fork();

                if (pid == 0) {
                        close(server_fd);
                        if (client_fd == -1) {
                                exit(0);
                        }

                        last_operation = clock();
                        char lines[24];
                        while (1) {
                                bzero(buffer, MAX_LIMIT);
                                FILE* file = fopen("DATA/tileset", "r");
                                if (file != NULL) {
                                        while (fgets(lines, sizeof(lines), file)) {

                                                printf("%s", lines);
                                                strncpy(buffer, lines, 24);
//                                        strncpy(buffer, "RETURN", 24);
                                                write(client_fd, buffer, strlen(buffer));
                                                break; 
                                }}
                                fclose(file);
/*
                                FILE *database;
                                database = fopen("DATA/database.txt", "a");
                                fprintf(database, "%s\n", buffer);
                                fclose(database);
*/
                                last_operation = clock();

                                break;
                                }
                        return EXIT_SUCCESS;
                }
        }
}
