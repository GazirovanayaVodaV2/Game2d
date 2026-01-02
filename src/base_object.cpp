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

bool global_check_collision(base_object* obj1, base_object* obj2)
{
	auto this_rect = obj1->get_pos().get_frect(obj1->get_size());
	auto object_rect = obj2->get_pos().get_frect(obj2->get_size());

	bool vertical_collision = ((this_rect.y + this_rect.h) > object_rect.y) &&
		(this_rect.y < (object_rect.y + object_rect.h));

	bool horizontal_collision = ((this_rect.x + this_rect.w) > object_rect.x) &&
		(this_rect.x < (object_rect.x + object_rect.w));
	auto collided = (vertical_collision && horizontal_collision);

	return collided;
}
