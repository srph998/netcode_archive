#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SCREEN_W 480
#define SCREEN_H 360
#define FONT "DATA/sazanami-gothic.ttf"
#define BG_COL 0,0,0,255
#define FG_COL 255,255,255,255

int main() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("[ERROR] SDL_INIT_VIDEOに失敗しました: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if (TTF_Init() < 0) {
		printf("[ERROR] TTF_Initに失敗しました: %s\n", TTF_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	SDL_Window *window = SDL_CreateWindow("ウィンドウ",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_W,SCREEN_H,0);

	if (!window){
		printf("[ERROR] SDL_CreateWindowに失敗しました: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer){
		printf("[ERROR] SDL_CreateRendererに失敗しました: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_SetRenderDrawColor(renderer, BG_COL);
	SDL_RenderClear(renderer);

	TTF_Font *font = TTF_OpenFont(FONT, 12);

	if (!font){
		printf("[ERROR] TTF_OpenFontに失敗しました: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}
	// メインループ
	while(1){
		SDL_Color textColor = {FG_COL};
		SDL_Surface *textSurface = TTF_RenderText_Solid(font, "test", textColor);

		if (!textSurface){
			printf("[ERROR] TTF_RenderText_Solidに失敗しました: %s\n", TTF_GetError());
			return EXIT_FAILURE;
		}

		SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

		if (!textTexture){
			printf("[ERROR] SDL_CreateTextureFromSurface に失敗しました: %s\n", SDL_GetError());
			return EXIT_FAILURE;
		}

		SDL_Rect textRect = {0,0,textSurface->w,textSurface->h};
		SDL_RenderCopy(renderer,textTexture,NULL,&textRect);
		SDL_RenderPresent(renderer);
		SDL_Delay(2000);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return EXIT_SUCCESS;
}
