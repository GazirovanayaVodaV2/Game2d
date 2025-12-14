#include "vector.h"

#include <cmath>

#include <utils.h>

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
	vec.x = x;
	vec.y = y;
}

float vec2::x()
{
	return vec.x;
}

float vec2::y()
{
	return vec.y;
}

SDL_Point vec2::get_int()
{
	return { convert::f2i(vec.x), convert::f2i(vec.y) };
}

vec2 vec2::operator+(const vec2& _vec)
{
	return vec2(this->vec.x + _vec.vec.x, this->vec.y + _vec.vec.y);
}

vec2 vec2::operator+=(const vec2& _vec)
{
	this->vec.x += _vec.vec.x;
	this->vec.y += _vec.vec.y;

	return *this;
}

vec2 vec2::operator-(const vec2& _vec)
{
	return vec2(this->vec.x - _vec.vec.x, this->vec.y - _vec.vec.y);
}

vec2 vec2::operator-=(const vec2& _vec)
{
	this->vec.x -= _vec.vec.x;
	this->vec.y -= _vec.vec.y;

	return *this;
}

vec2 vec2::operator*(const vec2& _vec)
{
	return vec2(this->vec.x * _vec.vec.x, this->vec.y * _vec.vec.y);
}

vec2 vec2::operator*(const float val)
{
	return vec2(this->vec.x * val, this->vec.y * val);
}

vec2 vec2::operator/(const vec2& _vec)
{
	return vec2(this->vec.x / _vec.vec.x, this->vec.y / _vec.vec.y);
}

vec2 vec2::operator/(const float val)
{
	return vec2(this->vec.x / val, this->vec.y / val);
}

float vec2::scalar(const vec2& _vec) const
{
	return (this->vec.x * _vec.vec.x) + (this->vec.y * _vec.vec.y);
}

float vec2::len() const
{
	return sqrtf(powf(this->vec.x, 2.0f) + powf(this->vec.y, 2.0f));
}

vec2 vec2::normalize() const
{
	return vec2(this->vec) / len();
}

SDL_Rect vec2::get_rect(const vec2& _vec)
{
	return { convert::f2i(x()), convert::f2i(y()), 
		convert::f2i(_vec.vec.x), convert::f2i(_vec.vec.y) };
}

SDL_FRect vec2::get_frect(const vec2& _vec)
{
	return {x(), y(), _vec.vec.x, _vec.vec.y};
}

vec2 vec2::operator=(const vec2& b)
{
	this->vec.x = b.vec.x;
	this->vec.y = b.vec.y;

	return *this;
}

vec2 vec2::operator=(const float b)
{
	this->vec.x = b;
	this->vec.y = b;

	return *this;
}
