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
#define BG_COL 0,0,200,255
#define FG_COL 0,0,0,255

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
                TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
        int text_w;
        int text_h;
        SDL_Surface *surface;
        SDL_Color textColor = {FG_COL};

        surface = TTF_RenderText_Blended_Wrapped(font, text, textColor, 256);
        *texture = SDL_CreateTextureFromSurface(renderer, surface);
        text_w = surface->w;
        text_h = surface->h;
        SDL_FreeSurface(surface);
        rect->x = x;
        rect->y = y;
        rect->w = text_w;
        rect->h = text_h;
}

int main(int argc, char *argv[]) {
        int sockfd, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        char buffer[MAX_LIMIT];
        char *font_path;

        SDL_Event event;
        SDL_Rect rect1, rect2;
        SDL_Renderer *renderer;
        SDL_Texture *texture1, *texture2;
        SDL_Window *window;
        int quit;

        font_path = "DATA/sazanami-gothic.ttf";
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
                perror("[ERROR] on conection");
                exit(1);
        }

        SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_W, 0, &window, &renderer);
        TTF_Init();
        TTF_Font *font = TTF_OpenFont(font_path, 12);
        if (font == NULL) {
                fprintf(stderr, "[ERROR] TTF_OpenFont\n");
                exit(EXIT_FAILURE);
        }
        while (1) {
        
        printf("%s\n", buffer);
        get_text_and_rect(renderer, 0, 0, "buffer", font, &texture1, &rect1);
        get_text_and_rect(renderer, 0, rect1.y + rect1.h, buffer, font, &texture2, &rect2);
        
        quit = 0;
        while (!quit) {
                while (SDL_PollEvent(&event) == 1) {
                        if (event.type == SDL_QUIT) {
                quit = 1;
                }
        }
        SDL_SetRenderDrawColor(renderer, BG_COL);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);

        SDL_RenderPresent(renderer);
        SDL_Delay(6000);
        }
        }

        SDL_DestroyTexture(texture1);
        SDL_DestroyTexture(texture2);
        TTF_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_SUCCESS;
}
