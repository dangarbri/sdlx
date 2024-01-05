#include <time.h>
#include <stdlib.h>
#include "sdlx.h"

// If compiling on windows in debug mode, enable memory leak detection
#if defined(_WIN32) && defined(_CRTDBG_MAP_ALLOC)
#include <crtdbg.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
static void EMCC_Loop(void);
#endif

SDLX_State sdlx = { 0 };

SDL_bool SDLX_Init(Uint32 sdl_flags, int img_flags)
{
	srand((int) time(NULL));
	if (sdl_flags == 0) { sdl_flags = SDL_INIT_EVERYTHING; }
	if (img_flags == 0) { img_flags = 0xFFFFFFFF;  }
	if (SDL_Init(sdl_flags) == 0) {
		if (TTF_Init() == 0) {
			if (IMG_Init(img_flags) != 0) {
#ifdef __EMSCRIPTEN__
				emscripten_set_main_loop(EMCC_Loop, 0, 0);
#endif
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
// In emscripten, this can just return.
// The browser will clean up everything when the app closes.
#ifndef __EMSCRIPTEN__
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
#if defined(_WIN32) && defined(_CRTDBG_MAP_ALLOC)
	_CrtDumpMemoryLeaks();
#endif
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

/**
 * Processes all events.
 * If quit event is detected, return SDL_FALSE
 */
static SDL_bool SDLX_ProcessEvents(EventHandler handler) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {  // poll until all events are handled!
		// If user has provided an event handler, run it.
		if (handler) {
			// If event handler returns SDL_FALSE, quit terminate the loop
			if (handler(&event) == SDL_FALSE) {
				return SDL_FALSE;
				break;
			}
		}

		// If quit is detected then signal that we should exit the loop.
		if (event.type == SDL_QUIT) {
			return SDL_FALSE;
			break;
		}
	}
	return SDL_TRUE;
}

static void SDLX_tick(MainLoop loop) {
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

static SDL_bool app_is_still_running = SDL_TRUE;
#ifndef __EMSCRIPTEN__
void SDLX_Loop(EventHandler handler, MainLoop loop)
{
	app_is_still_running = SDL_TRUE;
	while (app_is_still_running) {
		app_is_still_running = SDLX_ProcessEvents(handler);

		if (app_is_still_running) {
			SDLX_tick(loop);
		}
	}
}
#else // Emscripten support
// User provided loop set by SDLX_Loop
static MainLoop main_loop = NULL;
// User provided event handler set by SDLX_Loop
static EventHandler event_handler = NULL;
static void EMCC_Loop(void) {
	if (app_is_still_running) {
		SDLX_ProcessEvents(event_handler);
		if (main_loop) {
			SDLX_tick(main_loop);
		}
	}
}

void SDLX_Loop(EventHandler handler, MainLoop loop) {
	event_handler = handler;
	main_loop = loop;
}
#endif

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

float SDLX_Distance(const SDL_FPoint* a, const SDL_FPoint* b)
{
	float dx = SDL_fabsf(a->x - b->x);
	float dy = SDL_fabsf(a->y - b->y);
	return SDL_sqrtf(SDL_powf(dx, 2) + SDL_powf(dy, 2));
}
