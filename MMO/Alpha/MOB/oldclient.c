/*

*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define HOST "127.0.0.1"
#define PORTNO 1337
#define MAX_LIMIT 256

#define SCREEN_W 480
#define SCREEN_H 360
#define FONT "DATA/sazanami-gothic.ttf"
#define BG_COL 0,0,200,255
#define FG_COL 0,0,0,255

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

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                printf("[ERROR] SDL_INIT_VIDEO Error: %s\n", SDL_GetError());
                return EXIT_FAILURE;
        }

        if (TTF_Init() < 0) {
                printf("[ERROR] TTF_Init Error: %s\n", TTF_GetError());
                SDL_Quit();
                return EXIT_FAILURE;
        }

        SDL_Window *window =
                SDL_CreateWindow("Display", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, 0);

        if (!window) {
                printf("[ERROR] SDL_CreateWindow Error: %s\n", SDL_GetError());
                return EXIT_FAILURE;
        }

        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

        if (!renderer) {
                printf("[ERROR] SDL_CreateRenderer Error: %s\n",
                        SDL_GetError());
                return EXIT_FAILURE;
        }

        SDL_SetRenderDrawColor(renderer, BG_COL);
        SDL_RenderClear(renderer);

        TTF_Font *font = TTF_OpenFont(FONT, 12);

        if (!font) {
                printf("[ERROR] TTF_OpenFont Error: %s\n", TTF_GetError());
                return EXIT_FAILURE;
        }

        bzero((char *)&serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
                server->h_length);
        serv_addr.sin_port = htons(PORTNO);

        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
                perror("[ERROR] on conection");
                exit(1);
        }

        while (1) {

                while (read(sockfd, buffer, strlen(buffer))); {
                        printf("test");
                        SDL_Color textColor = {FG_COL};
                        SDL_Surface *textSurface =
                           TTF_RenderText_Solid(font, buffer, textColor);

                        SDL_Texture *textTexture =
                                SDL_CreateTextureFromSurface(renderer, textSurface);

                   if (!textTexture) {
                                printf("[ERROR] SDL_CreateTextureFromSurface "
                                        "Error: %s\n",
                                        SDL_GetError());
                             return EXIT_FAILURE;
                        }
                 SDL_Rect textRect = {0, 0, textSurface->w, textSurface->h};
                 SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                 SDL_RenderPresent(renderer);
                 SDL_Delay(6000);
                }
                SDL_Delay(6000);
                //SDL_RenderClear(renderer);
                //break;
        }

        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();

        return EXIT_SUCCESS;
        return 0;
}
