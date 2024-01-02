#pragma once

#include <assert.h>

#include "sdlx.h"

int main(int argc, char** argv) {
	// left is 5, right is 5.00001f, error is 0.0001f. Since 5.00001 is between 4.9999 and 5.0001, it is true
	assert(SDLX_FloatEq(5.0f, 5.00001f, 0.0001f));
	assert(SDLX_FloatEq(5, 6, 2));
	assert(!SDLX_FloatEq(1, 2, 0.1f));
	return 0;
}