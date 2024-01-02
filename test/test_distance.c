#include <assert.h>

#include "sdlx.h"

int main(int argc, char** argv) {
	SDL_FPoint left = { 0, 0 };
	SDL_FPoint right = { 5, 0 };
	assert(SDLX_FloatEq(SDLX_Distance(&left, &right), 5.0f, 0.0001));

	right.x = 3;
	right.y = 4;
	assert(SDLX_FloatEq(SDLX_Distance(&left, &right), 5.0f, 0.0001));

	right.x = 0;
	right.y = 5;
	assert(SDLX_FloatEq(SDLX_Distance(&left, &right), 5.0f, 0.0001));
	assert(!SDLX_FloatEq(SDLX_Distance(&left, &right), 7.0f, 0.0001));
	return 0;
}