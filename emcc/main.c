#include "sdlx.h"

void Loop(float dt) {
	SDL_Color black = { 0, 0, 0, 0xFF };
	SDLX_SetRenderDrawColor(sdlx.renderer, black);
	SDL_RenderClear(sdlx.renderer);

	SDL_Color red = { 0xFF, 0, 0, 0xFF };
	SDLX_SetRenderDrawColor(sdlx.renderer, red);
	SDL_Rect rect = { 255, 100, 200, 100 };
	SDL_RenderFillRect(sdlx.renderer, &rect);
	SDL_RenderPresent(sdlx.renderer);
}

int main(int argc, char** argv) {
	if (SDLX_Init(SDL_INIT_VIDEO, IMG_INIT_PNG)
		&& SDLX_CreateWindow(711, 400, 0)) {
		puts("Calling SDLX_Loop");
		SDLX_Loop(NULL, Loop);
		SDLX_Quit();
	}
	return 0;
}