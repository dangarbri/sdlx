# SDLX

SDL extensions wrap some common SDL functions to make the library a little more friendly.

## Usage

To include in your project, add the following to your `CMakeLists.txt`.
You will also need the dependencies for sdlx: SDL2, SDL2_ttf, and SDL2_image.

```cmake
include(FetchContent)
include(CMakeFindDependencyMacro)
FetchContent_Declare(
  sdlx
  GIT_REPOSITORY https://github.com/dangarbri/sdlx.git
  GIT_TAG e63e6d379a846be993c2ff356cba5594dbd53601 # Replace with desired commit
)
FetchContent_MakeAvailable(sdlx)
include(${sdlx_BINARY_DIR}/SDLXTargets.cmake)

...

find_dependency(SDL2)
target_include_directories(<your target> PRIVATE SDLX::sdlx)
target_link_libraries(<your target> SDLX::sdlx)
```

## Reference

```
SDL_bool SDLX_Init(void);
```

Initializes all SDL subsystems. Should be the first call to SDLX.
Alternatively you can initialize SDL itself.
Use `SDL_Quit` as usual to shutdown the subsystems.

```
typedef SDL_bool (*EventHandler)(SDL_Event*);
typedef void (*MainLoop)(float dt);
void SDLX_Loop(EventHandler* handler, MainLoop* loop);
```

Wraps your application's main function with built-in event processing and delta time.
You provide the event processor which gets all received SDL_Events.
Once events are processed, loop is called with the number of seconds since the last frame.
This function internally handles the exit event.

You terminate this loop by having your event handler return `SDL_FALSE`.

## Window Functions

```
SDL_bool SDLX_CreateWindow(int width, int height, Uint32 flags);
```

Creates a window and renderer and assigns them to `sdlx`.

## Render functions

```
int SDLX_RenderClear(SDL_Renderer* renderer, SDL_Color color);
#define SDLX_SetRenderDrawColor(renderer, color) SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)
```

`SDLX_RenderClear` allows you to clear the renderer with a specified color.

`SDLX_SetRenderDrawColor(renderer, color)` is a shorthand for setting the draw color. Saves a lot of typing.

## Mouse Functions

```
SDL_bool SDLX_MouseInWindow(SDL_Window* window);
```

Checks if the mouse is inside the given window.

## Application State

```
typedef struct {
	/** Window */
	SDL_Window* window;
	/** Window renderer */
	SDL_Renderer* renderer;
} SDLX_State;

extern SDLX_State sdlx;
```

Some variables are commonly needed throughout the SDL application such
as a reference to the primary renderer. You can place your renderer in
`sdlx` so it can be accessed throughout your application.

## Colors

`SDLX_ColorsMatch` does a comparison for two SDL_Colors.

## Error Handling

```
void SDLX_perror(char* message);
```

Like standard C's perror but for SDL.

## Logging

SDLX adds a new log categories

- `SDLX_LOG_CATEGORY_PERFORMANCE` - When priority is set to Debug, this logs the current FPS to the console.

## Randomness

SDLX calls `srand` during SDLX_Init and provides `SDL_RandInt` for choosing random numbers.

## Math

You can compare floats with `SDLX_FloatEq` to handle comparing floats.
This is good for when you need to compare float math with some rounding errors.
i.e. `5 == 5.000000001` would be false, but `SDLX_FloatEq(5, 5.000000001, 0.00001)` is true.
The 3rd argument is the tolerance for how far apart the numbers need to be to be considered not equal.

`SDL_Distance` can be used to compute the distance between 2 `SDL_FPoint`s

