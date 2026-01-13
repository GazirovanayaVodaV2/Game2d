#include "base_object.h"

void base_object::input_if_selected(const SDL_Event* event)
{
	if ((event->type == SDL_EVENT_KEY_DOWN) and
		selected) {
		switch (event->key.key)
		{
		case SDLK_LEFT: {
			pos.x -= 1;
		} break;
		case SDLK_RIGHT: {
			pos.x += 1;
		} break;
		case SDLK_UP: {
			pos.y -= 1;
		} break;
		case SDLK_DOWN: {
			pos.y += 1;
		} break;
		default: {
		} break;
		}
	}
}


