#include "rgba.h"

rgba::rgba(byte r, byte g, byte b)
{
	*this = rgba(r, g, b, 255);
}

rgba::rgba(byte r, byte g, byte b, byte a)
{
	this->color = { r,g,b,a };
}

rgba::rgba(SDL_Color clr)
{
	*this = rgba(clr.r, clr.g, clr.b, clr.a);
}

rgba::rgba(hex clr)
{

	byte r = static_cast<byte>((clr & 0xff000000) >> 24);
	byte g = static_cast<byte>((clr & 0x00ff0000) >> 16);
	byte b = static_cast<byte>((clr & 0x0000ff00) >> 8);
	byte a = static_cast<byte>(clr & 0x000000ff);

	*this = rgba(r, g, b, a);
}

int rgba::to_hex()
{
	int res=0;
	res |= color.r << 24;
	res |= color.g << 16;
	res |= color.b << 8;
	res |= color.a;

	return res;
}

rgba rgba::mix(rgba clr, float factor)
{
	return rgba{
	static_cast<byte>(this->color.r + (clr.color.r - this->color.r) * factor),
	static_cast<byte>(this->color.g + (clr.color.g - this->color.g) * factor),
	static_cast<byte>(this->color.b + (clr.color.b - this->color.b) * factor),
	static_cast<byte>(this->color.a + (clr.color.a - this->color.a) * factor)
	};
}
