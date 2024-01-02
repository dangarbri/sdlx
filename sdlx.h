#pragma once
// If compiling on windows in debug mode, enable memory leak detection
#if !defined(NDEBUG) && defined(_WIN32) && defined(_CRTDBG_MAP_ALLOC)
#include <crtdbg.h>
#endif

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

/** Map SDL memory allocations to CRTDBG allocations */
#if !defined(NDEBUG) && defined(_WIN32) && defined(_CRTDBG_MAP_ALLOC)
#define SDL_malloc(size) malloc(size)
#define SDL_calloc(count, size) calloc(count, size)
#define SDL_free(ptr) free(ptr)
#endif

typedef enum {
	SDLX_LOG_CATEGORY_PERFORMANCE = SDL_LOG_CATEGORY_CUSTOM,
} SDLX_LogCategory;

typedef struct {
	/** Window */
	SDL_Window* window;
	/** Window renderer */
	SDL_Renderer* renderer;
} SDLX_State;

extern SDLX_State sdlx;

/**
 * Initializes all SDL subsystems.
 * This is equivalent to SDL_Init(SDL_INIT_EVERYTHING);
 * Prints an error message on failure
 * @returns SDL_TRUE on success or SDL_FALSE
 */
SDL_bool SDLX_Init(void);

/**
 * Quit all SDL extensions and subsystems
 */
void SDLX_Quit(void);

/**
 * Creates the window and renderer and assigns them to sdlx.
 * @returns SDL_TRUE on success, else SDL_FALSE
 */
SDL_bool SDLX_CreateWindow(int width, int height, Uint32 flags);

/**
 * If an error occurred in an SDL function, this will print the error message to stderr
 * Behaves like c's perror, but for SDL errors.
 * @param message Message to output before the SDL error
 */
void SDLX_perror(char* message);

/**
 * Processes events.
 * @returns SDL_FALSE to quit the program
 */
typedef SDL_bool (*EventHandler)(SDL_Event*);

/**
 * Application loop function
 * @param dt Seconds since last frame
 */
typedef void (*MainLoop)(float dt);

/**
 * Handles the application's main loop.
 * All events are forwarded to the provided event handler.
 * After events are processed, the MainLoop function is called.
 */
void SDLX_Loop(EventHandler handler, MainLoop loop);

/**
 * Clears the renderer with the given color
 * @param color Render clear color
 * @returns 0 on success, negative value on error. Internally prints to stderr on error.
 */
int SDLX_RenderClear(SDL_Renderer* renderer, SDL_Color color);

/**
 * Sets the render draw color for the renderer referenced by sdlx.renderer.
 * @param color New draw color
 */
#define SDLX_SetRenderDrawColor(renderer, color) SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)

/**
 * Checks if the mouse is currently inside the window
 * @returns SDL_TRUE if the mouse is inside the window, else SDL_FALSE
 */
SDL_bool SDLX_MouseInWindow(SDL_Window* window);

// @returns value between min and max inclusive
Uint32 SDLX_RandInt(Uint32 min, Uint32 max);

/**
 * Check if two colors are equal
 * @param a SDL_Color
 * @param b SDL_Color
 */
#define SDLX_ColorsMatch(a, b) (*((Uint32*)&a) == *((Uint32*)&b))

#define SDLX_SafeFree(ptr) if (ptr) { SDL_free(ptr); ptr = NULL; }

/**
 * Compares two floats and returns whether they are equal (or close to each other)
 * The error value is used to designate how far apart two floats could be and still be considered equal.
 * @param a first value
 * @param b second value
 * @param error Maximum difference from a value for two floats to be considered equal
 */
#define SDLX_FloatEq(a, b, error) ((b >= (a-error)) && (b <= (a+error)))

/**
 * Computes the distance between two points
 */
float SDLX_Distance(SDL_FPoint* a, SDL_FPoint* b);