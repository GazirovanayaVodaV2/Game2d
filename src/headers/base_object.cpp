#include "base_object.h"

void base_object::input_if_selected(const SDL_Event* event)
{
	if ((event->type == SDL_EVENT_KEY_DOWN) and
		selected) {
		switch (event->key.key)
		{
		case SDLK_LEFT: {
			pos.vec.x -= 1;
		} break;
		case SDLK_RIGHT: {
			pos.vec.x += 1;
		} break;
		case SDLK_UP: {
			pos.vec.y -= 1;
		} break;
		case SDLK_DOWN: {
			pos.vec.y += 1;
		} break;
		default: {
		} break;
		}
	}
}
