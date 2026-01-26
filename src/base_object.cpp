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

float base_object::get_distance(base_object* obj)
{
	return sqrtf(get_sqr_distance(obj));
}

float base_object::get_sqr_distance(base_object* obj)
{
	auto this_center = pos + size / 2.0f;
	auto obj_center = obj->get_pos() + obj->get_size() / 2.0f;

	auto delta = this_center - obj_center;

	return delta.sqr_len();
}

float base_object::get_size_radius()
{
	return sqrtf((size.x * size.x) + (size.y * size.y)) / 2.0f;
}


