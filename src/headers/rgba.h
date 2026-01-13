#pragma once
#include <SDL3/SDL.h>

#include <utils.h>

#include <ctype.h>

using hex = uint32_t;

enum colors: unsigned int {
	WHITE = 0xffffffff,
	BLACK = 0x000000ff,
	RED   = 0xff0000ff,
	GREEN = 0x00ff00ff,
	BLUE  = 0x0000ffff
};

struct rgba
{
	SDL_Color color = { NULL };
	rgba() {};
	rgba(byte r, byte g, byte b);
	rgba(byte r, byte g, byte b, byte a);
	rgba(SDL_Color clr);

	rgba(hex clr);

	int to_hex();

	~rgba() = default;
};

