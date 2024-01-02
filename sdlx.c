#include <time.h>
#include <stdlib.h>
#include "sdlx.h"

// If compiling on windows in debug mode, enable memory leak detection
#if defined(_WIN32) && defined(_CRTDBG_MAP_ALLOC)
#include <crtdbg.h>
#endif

SDLX_State sdlx = { 0 };

SDL_bool SDLX_Init(void)
{
	srand((int) time(NULL));
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		if (TTF_Init() == 0) {
			if (IMG_Init(0xFFFFFFFF) != 0) {
				return SDL_TRUE;
			}
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_image: %s\n", IMG_GetError());
			TTF_Quit();
		}
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_ttf: %s\n", TTF_GetError());
		SDL_Quit();
	}
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s\n", SDL_GetError());
	return SDL_FALSE;
}

void SDLX_Quit(void)
{
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
#if defined(_WIN32) && defined(_CRTDBG_MAP_ALLOC)
	_CrtDumpMemoryLeaks();
#endif
}

SDL_bool SDLX_CreateWindow(int width, int height, Uint32 flags)
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	if (SDL_CreateWindowAndRenderer(width, height, flags, &window, &renderer) == 0) {
		sdlx.window = window;
		sdlx.renderer = renderer;
		return SDL_TRUE;
	}
	SDLX_perror(NULL);
	return SDL_FALSE;
}

void SDLX_perror(char* message)
{
	if (message) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s: ", message);
	}
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", SDL_GetError());
}

void SDLX_Loop(EventHandler handler, MainLoop loop)
{
	SDL_bool app_is_still_running = SDL_TRUE;
	while (app_is_still_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {  // poll until all events are handled!
			if (event.type == SDL_QUIT) {
				app_is_still_running = SDL_FALSE;
				break;
			}
			// If user has provided an event handler, run it.
			if (handler) {
				// If event handler returns SDL_FALSE, quit terminate the loop
				if (handler(&event) == SDL_FALSE) {
					app_is_still_running = SDL_FALSE;
					break;
				}
			}
		}

		if (app_is_still_running) {
			// Compute deltatime
			static Uint64 clock = 0;
			Uint64 now = SDL_GetTicks64();
			Uint64 delta = now - clock;
			float dt = delta / 1000.0f;
			clock = now;
			SDL_LogDebug(SDLX_LOG_CATEGORY_PERFORMANCE, "Delta Time: %f\n", dt);

			// update game state, draw the current frame
			loop(dt);
		}
	}
}

int SDLX_RenderClear(SDL_Renderer* renderer, SDL_Color color)
{
	int res = 0;
	res += SDLX_SetRenderDrawColor(renderer, color);
	res += SDL_RenderClear(renderer);
	if (res < 0) {
		SDLX_perror(NULL);
	}
	return res;
}

SDL_bool SDLX_MouseInWindow(SDL_Window* window)
{
	SDL_Point mouse_pos;
	SDL_GetGlobalMouseState(&mouse_pos.x, &mouse_pos.y);
	SDL_Rect window_rect;
	SDL_GetWindowPosition(window, &window_rect.x, &window_rect.y);
	SDL_GetWindowSize(window, &window_rect.w, &window_rect.h);

	return SDL_PointInRect(&mouse_pos, &window_rect);
}

Uint32 SDLX_RandInt(Uint32 min, Uint32 max)
{
	return ((Uint32) rand() % ((max + 1) - min)) + min;
}

float SDLX_Distance(SDL_FPoint* a, SDL_FPoint* b)
{
	float dx = SDL_fabsf(a->x - b->x);
	float dy = SDL_fabsf(a->y - b->y);
	return SDL_sqrtf(SDL_powf(dx, 2) + SDL_powf(dy, 2));
}
