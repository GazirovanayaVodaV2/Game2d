#include "vector.h"

#include <cmath>

#include "Utils.h"

vec2::vec2(SDL_FPoint sdl_fpoint)
{
	*this = vec2(sdl_fpoint.x, sdl_fpoint.y);
}

vec2::vec2(SDL_Point sdl_point)
{
	*this = vec2(convert::i2f(sdl_point.x), convert::i2f(sdl_point.y));
}

vec2::vec2(int x, int y)
{
	*this = vec2(convert::i2f(x), convert::i2f(y));
}

vec2::vec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

SDL_Point vec2::get_int()
{
	return { convert::f2i(x), convert::f2i(y) };
}

vec2 vec2::operator+(const vec2& _vec)
{
	return vec2(this->x + _vec.x, this->y + _vec.y);
}

vec2 vec2::operator+=(const vec2& _vec)
{
	this->x += _vec.x;
	this->y += _vec.y;

	return *this;
}

vec2 vec2::operator-(const vec2& _vec)
{
	return vec2(this->x - _vec.x, this->y - _vec.y);
}

vec2 vec2::operator-=(const vec2& _vec)
{
	this->x -= _vec.x;
	this->y -= _vec.y;

	return *this;
}

vec2 vec2::operator*(const vec2& _vec)
{
	return vec2(this->x * _vec.x, this->y * _vec.y);
}

vec2 vec2::operator*(const float val)
{
	return vec2(this->x * val, this->y * val);
}

vec2 vec2::operator/(const vec2& _vec)
{
	return vec2(this->x / _vec.x, this->y / _vec.y);
}

vec2 vec2::operator/(const float val)
{
	return vec2(this->x / val, this->y / val);
}

float vec2::scalar(const vec2& _vec) const
{
	return (this->x * _vec.x) + (this->y * _vec.y);
}

float vec2::len() const
{
	return sqrtf(sqr_len());
}

float vec2::sqr_len() const
{
	return powf(this->x, 2.0f) + powf(this->y, 2.0f);
}

vec2 vec2::normalize() const
{
	return vec2(x, y) / len();
}

SDL_Rect vec2::get_rect(const vec2& _vec)
{
	return { convert::f2i(x), convert::f2i(y), 
		convert::f2i(_vec.x), convert::f2i(_vec.y) };
}

SDL_FRect vec2::get_frect(const vec2& _vec)
{
	return {x, y, _vec.x, _vec.y};
}

vec2 vec2::operator=(const vec2& b)
{
	this->x = b.x;
	this->y = b.y;

	return *this;
}

vec2 vec2::operator=(const float b)
{
	this->x = b;
	this->y = b;

	return *this;
}
