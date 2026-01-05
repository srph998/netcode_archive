#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
	int sockfd, portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2) {
		fprintf(stderr, "ERROR no port given\n");
		exit(1);
	}

	int n;
	char buffer[MAX_SIZE];
	bzero(buffer, MAX_SIZE);
	n = read(sockfd, buffer, MAX_SIZE - 1);
	if (n < 0) {
		perror("ERROR reading from socket\n");
		exit(1);
	}
	printf("из: ");
	bzero(buffer, MAX_SIZE);
	fgets(buffer, MAX_SIZE - 1, stdin);
	n = write(sockfd, buffer, strlen(buffer));
}

