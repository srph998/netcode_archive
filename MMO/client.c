#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SCREEN_W 480
#define SCREEN_H 480

#define BG_COL 0,0,0,255
#define FG_COL 0,255,0,255

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
        char *font_path;

        SDL_Event event;
        SDL_Rect rect1;
        SDL_Renderer *renderer;
        SDL_Texture *texture1;
        SDL_Window *window;
        int quit;

        font_path = "files/sazanami-gothic.ttf";
        
        SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_W, 0, &window, &renderer);
        TTF_Init();
        TTF_Font *font = TTF_OpenFont(font_path, 12);
        if (font == NULL) {
                fprintf(stderr, "[ERROR] TTF_OpenFont Failed\n");
                exit(EXIT_FAILURE);
        }

        char line[100][100];
        char fname[20];
        FILE *tilemap;
        int i = 0;
        scanf("%s",fname);
        tilemap = fopen("files/tileset", "r");
        while (1){
        while(fgets(line[i], 100, tilemap))
        {
                line[i][strlen(line[i]) - 1] = '\0';
                i++;
        }
        
        
        get_text_and_rect(renderer,0,0,line[50],font,&texture1,&rect1);
        
        SDL_SetRenderDrawColor(renderer, BG_COL);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        //SDL_RenderCopy(renderer, texture2, NULL, &rect2);
        SDL_RenderPresent(renderer);
        SDL_Delay(6000);

        }
        SDL_DestroyTexture(texture1);
        //SDL_DestroyTexture(texture2);
        TTF_Quit();

        SDL_DestroyRenderer(renderer);
        return EXIT_SUCCESS;
}
