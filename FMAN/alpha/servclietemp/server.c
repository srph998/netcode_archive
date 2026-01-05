#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_SIZE 1024

void dostuff(int);

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno, pid;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2) {
		fprintf(stderr, "ERROR no port given\n");
		exit(1);
	}

	// create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	// init sock struct
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// bind host addr
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("ERROR on bind host addr");
		exit(1);
	}
	
	// listen for clients
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	while (1) {

		// accept connection from client
		newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
		if (newsockfd < 0) {
			perror("ERROR on accept");
			exit(1);
		}
	
		// create new process
		pid = fork();
		if (pid < 0) {
			perror("ERROR on fork");
			exit(1);
		}

		if (pid == 0) {
			// client
			close(sockfd);
			dostuff(newsockfd);
			exit(0);
		} else {
			// server
			close(newsockfd);
		}
		
	}

	return 0;
}

void dostuff(int sock) {
	int n;
	char buffer[MAX_SIZE];

	bzero(buffer, MAX_SIZE);
	n = read(sock, buffer, MAX_SIZE - 1);
	if (n < 0) {
		perror("ERROR reading from socket");
		exit(1);
	}

	printf("В: %s\n", buffer);

	n = write(sock, "RET: Y", 6);
	if (n < 0) {
		perror("ERROR writing to socket");;
		exit(1);
	}
}
