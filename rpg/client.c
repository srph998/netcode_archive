/*
	client.c
	by _______
	@????

	in an operable environment, instead of throwing an exit(1), the
	code would instead point to either the end of the stack, there-
	fore running the self-destruct code. alternatively, the self-
	destruct code may become it's own function.
*/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

// GLOBAL
#define HOST "127.0.0.1"
#define PORTNO 1337
#define MAX_LIMIT 256

int main(int argc, char *argv[]) {
	// variable setup
	int sockfd, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[MAX_LIMIT];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server = gethostbyname(HOST);

	if (server == NULL) {
		fprintf(stderr,"[ERROR] can't locate host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(PORTNO);

	// server connection
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("[ERROR] on connection");
		exit(1);
	}

	// operation
	while (1) {
		// reset buffer, run logic 
		bzero(buffer, MAX_LIMIT);
		strncpy(buffer, "Template return data", 24);

		/*
		RUN SLOTTED CODE HERE
		the logic used for what to
		write to the buffer.

		could import file globally, set
		code to a function, call that
		function here.
		*/

		// write to buffer (send data)
		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) {
			perror("[ERROR] while writing to socket");
			exit(1);
		}

		/*
		RUN SELF DESTRUCT HERE
		the logic used for cleaning
		and removing the executable
		file from the machine (one time use)
		*/

		break;
	}
	return 0;
}
