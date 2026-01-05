#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	portno = 1337;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server = gethostbyname("127.0.0.1");

	if (server == NULL) {
		fprintf(stderr,"[ERROR] can't locate host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	// server connection
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("[ERROR] on connection");
		exit(1);
	}

	while (1) {
		printf("Send: ");
		bzero(buffer,256);
		scanf("%s", buffer);

		n = write(sockfd,buffer,strlen(buffer));
		if (n < 0) {
			perror("[ERROR] while writing to socket");
			exit(1);
		}
		bzero(buffer,256);
		n = read(sockfd, buffer, 255);
		if (n < 0) {
			perror("[ERROR] while reading from socket");
			exit(1);
		}
		printf("BROADCAST: %s\n", buffer);
		if (!bcmp(buffer, "quit", 4))
			break;
	}
	return 0;
}
