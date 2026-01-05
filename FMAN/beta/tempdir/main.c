#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SCREEN_W 480
#define SCREEN_H 360
#define MY_FONT "DATA/sazanami-gothic.ttf"

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("[ERROR] INIT FAIL SDL: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	if (TTF_Init() < 0) {
		printf("[ERROR] TTF_Error: %s\n", TTF_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_Window *window = SDL_CreateWindow("Window",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_W,SCREEN_H,0);

	if (!window){
		printf("[ERROR] Window open fail: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer){
		printf("[ERROR] Render error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	SDL_RenderClear(renderer);

	TTF_Font *font = TTF_OpenFont(MY_FONT, 12);

	if (!font){
		printf("[ERROR] Font error: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}

	while(1){
		SDL_Color textColor = {255,255,255,255}; 
		SDL_Surface *textSurface = TTF_RenderText_Solid(font, "test", textColor);

		if (!textSurface) {
			printf("[ERROR] Text surface error: %s\n", TTF_GetError());
			return EXIT_FAILURE;
		}

		// Create texture from surface
		SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		if (!textTexture){
			printf("[ERROR] Text texture error: %s\n", SDL_GetError());
			return EXIT_FAILURE;
		}

  		// Render text
		SDL_Rect textRect = {50, 50, textSurface->w, textSurface->h}; // rectangle where the text is drawn 
		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

		SDL_RenderPresent(renderer);
		SDL_Delay(2000);
	}

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();

  return EXIT_SUCCESS;
}
