/*
	server.c
	by hu5hbot

	this program requires instances of the (client.c) program
	to do anything other than sit there and run pretty.
*/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

// GLOBAL
#define PORTNO 1337
#define MAX_LIMIT 256

void bzero(void *a, size_t n) {
	memset(a, 0, n);
}

void bcopy(const void *src, void *dest, size_t n) {
	memmove(dest, src, n);
}

// structure setup
struct sockaddr_in* init_sockaddr_in(uint16_t port_number) {
	struct sockaddr_in *socket_address = malloc(sizeof(struct sockaddr_in));
	memset(socket_address, 0, sizeof(*socket_address));
	socket_address -> sin_family = AF_INET;
	socket_address -> sin_addr.s_addr = htonl(INADDR_ANY);
	socket_address -> sin_port = htons(port_number);
	return socket_address;
}
// math
char* process_operation(char *input) {
	size_t n = strlen(input) * sizeof(char);
	char *output = malloc(n);
	memcpy(output, input, n);
	return output;
}

int main(int argc, char *argv[] ) {
	// variable setup
	const uint16_t port_number = PORTNO;
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in *server_sockaddr = init_sockaddr_in(port_number);
	struct sockaddr_in *client_sockaddr = malloc(sizeof(struct sockaddr_in));
	socklen_t server_socklen = sizeof(*server_sockaddr);
	socklen_t client_socklen = sizeof(*client_sockaddr);

	// catchall error handling, can expand
	if (bind(server_fd, (const struct sockaddr*) server_sockaddr, server_socklen) < 0)
	{
		printf("[ERROR] Bind failure\n");
		exit(0);
	}
	if (listen(server_fd, 3) < 0)
	{
		printf("[ERROR] Listen failure\n");
		exit(0);
	}

	// more variables
	const size_t buffer_len = MAX_LIMIT;
	char *buffer = malloc(buffer_len * sizeof(char));
	char *response = NULL;
	time_t last_operation;
	__pid_t pid = -1;

	// client operation handling
	while (1) {
		int client_fd = accept(server_fd, (struct sockaddr *) &client_sockaddr, &client_socklen);
	// fork clients appropriately
	pid = fork();

	if (pid == 0) {
		close(server_fd);
		if (client_fd == -1) {
			exit(0);
		}

		last_operation = clock();

		while (1) {
			// handle closed connections
			read(client_fd, buffer, buffer_len);
			if (buffer == "close") {
				printf("ID@%d: ", getpid());
				close(client_fd);
				printf("Session @%d closed\n", client_fd);
				break;
			}
			// fancy "if process closes" math, can comment if too loud
			if (strlen(buffer) == 0) {
				clock_t d = clock() - last_operation;
				double dif = 1.0 * d / CLOCKS_PER_SEC;
				if (dif > 5.0) {
					printf("process@%d ", getpid());
					close(client_fd);
					printf("Timed out after %.3f seconds.\n", dif);
					break;
				}
				continue;
			}
			
			// recieve - display
			printf("!PING\n");
			printf("client@%d: ", getpid());
			printf("%s\n", buffer);
			
			if (strcmp(buffer, "break") == 0) {
				printf("done brooke");
			} else {
				printf("PEN");
			}
	
			// create folder / write data
			FILE *database;
			database = fopen("DATA/database.txt", "a");
			fprintf(database, "%s\n", buffer);
			fclose(database);

			// send response
			free(response);
			response = process_operation(buffer);
			bzero(buffer, buffer_len * sizeof(char));
	
			send(client_fd, response, strlen(response), 0);
			printf("!PONG\n\n");
			}
			
			last_operation = clock();

			exit(0);
		}
		else {
			close(client_fd);
		}
	}
}
