/*
   server.c
   FMAN-DEV
   NETseraph
   */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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
#define SCREEN_W 480
#define SCREEN_H 360
#define BG_COL 0,0,0,255
#define FG_COL 0,255,0,255

void bzero(void *a, size_t n) { memset(a, 0, n); }

void bcopy(const void *src, void *dest, size_t n) { memmove(dest, src, n); }

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
	int text_w
		int text_h;
	SDL_Surface *surface;
	SDL_Color textColor = {FG_COL};

	surface = TTF_RenderText_Blended_Wrapped(font,text,textColor,128);
	*texture = SDL_CreateTextureFromSurface(renderer, surface);

	text_w = surface->w;
	text_h = surface->h;
	SDL_FreeSurface(surface);
	rect->x = x;
	rect->y = y;
	rect->w = text_w;
	rect->h = text_h;
}

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

	if (bind(server_fd, (const struct sockaddr *)server_sockaddr,
				server_socklen) < 0) {
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

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("[ERROR] SDL_INIT_VIDEO Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (TTF_Init() < 0) {
		printf("[ERROR] TTF_Init Error: %s\n", TTF_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	char *font_path;
	SDL_Event event;
	SDL_Rect rect1;
	SDL_Renderer *renderer;
	SDL_Texture *texture1;
	SDL_Window *window
		int quit;

	font_path = "DATA/sazanami-gothic.ttf";

	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &window, &renderer);
	TTF_Init();
	TTF_Font *font = TTF_OpenFont(font_path, 12);
	if (font == NULL) {
		fprintf(stderr, "[ERROR] TTF_OpenFont Failed\n");
		exit(EXIT_FAILURE);
	}

	SDL_SetRenderDrawColor(renderer, BG_COL);
	SDL_RenderClear(renderer);

	// Start
	while (1) {
		int client_fd = accept(server_fd, (struct sockaddr *)&client_sockaddr,
				&client_socklen);

		pid = fork();

		if (pid == 0) {
			close(server_fd);
			if (client_fd == -1) {
				exit(0);
			}
			last_operation = clock();
			while (1) {
				read(client_fd, buffer, buffer_len);
				if (buffer == "close") {
					printf("ID@%d: ", getpid());
					close(client_fd);
					printf("Session @%d closed\n", client_fd);
					break;
				}
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

				printf("client@%d: ", getpid());
				printf("%s\n", buffer);
				/*
				   FILE *database;
				   database = fopen("DATA/database.txt", "a");
				   fprintf(database, "%s\n", buffer);
				   fclose(database);
				   */
				SDL_Color textColor = {FG_COL};
				SDL_Surface *textSurface =
					TTF_RenderText_Blended_Wrapped(font, buffer, textColor, 4);

				free(response);
				response = process_operation(buffer);
				bzero(buffer, buffer_len * sizeof(char));

				send(client_fd, response, strlen(response), 0);
				printf("!PONG\n\n");

				last_operation = clock();


				SDL_Texture *textTexture =
					SDL_CreateTextureFromSurface(renderer, textSurface);

				SDL_Rect textRect = {0, 0, textSurface->w, textSurface->h};
				SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
				SDL_RenderPresent(renderer);
				SDL_Delay(2000);
			}

			SDL_DestroyWindow(window);
			SDL_DestroyRenderer(renderer);
			SDL_Quit();

			return EXIT_SUCCESS;
		}
	}
}
