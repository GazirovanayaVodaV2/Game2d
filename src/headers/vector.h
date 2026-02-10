#pragma once

#include <SDL3/SDL.h>

struct vec2
{
	float x = 0.0f, y = 0.0f;

	vec2() {};
	vec2(SDL_FPoint sdl_fpoint);
	vec2(SDL_Point sdl_point);
	vec2(int x, int y);
	vec2(float x, float y);

	SDL_Point get_int();

	vec2 operator + (const vec2& _vec);
	vec2 operator += (const vec2& _vec);
	vec2 operator - (const vec2& _vec);
	vec2 operator -= (const vec2& _vec);

	vec2 operator * (const vec2& _vec);
	vec2 operator * (const float val);

	vec2 operator / (const vec2& _vec);
	vec2 operator / (const float val);

	vec2 operator=(const vec2& b);
	vec2 operator=(const float b);

	float scalar(const vec2& _vec) const;
	float len() const;
	float sqr_len() const;

	vec2 normalize() const;

	SDL_Rect get_rect(const vec2& _vec);
	SDL_FRect get_frect(const vec2& _vec);

	operator SDL_FPoint() {
		return { x, y };
	}
};
