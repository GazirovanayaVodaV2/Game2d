#include "game_object.h"
#include "window.h"

void game_object::draw_selection()
{
	auto cam_pos = camera::get_pos();
	auto render = camera::get();
	if (selected) {
		auto dest_rect = pos.get_frect(size);

		dest_rect.x = (pos.x + cam_pos.x - 2);
		dest_rect.y = (pos.y + cam_pos.y - 2);

		dest_rect.w += 4;
		dest_rect.h += 4;

		SDL_Color buffered_color;
		SDL_GetRenderDrawColor(render,
			&buffered_color.r,
			&buffered_color.g,
			&buffered_color.b,
			&buffered_color.a
		);

		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
		SDL_RenderRect(render, &dest_rect);
		SDL_SetRenderDrawColor(render,
			buffered_color.r,
			buffered_color.g,
			buffered_color.b,
			buffered_color.a);
	}
}

void game_object::draw_debug()
{
	auto viewport = camera::get_viewport();
	auto render = camera::get();
	if (exist) {
		auto dest_rect = pos.get_frect(size);

		dest_rect.x = (pos.x + viewport.x - 2);
		dest_rect.y = (pos.y + viewport.y - 2);

		dest_rect.w += 4;
		dest_rect.h += 4;

		SDL_Color buffered_color;
		SDL_GetRenderDrawColor(render,
			&buffered_color.r,
			&buffered_color.g,
			&buffered_color.b,
			&buffered_color.a
		);

		SDL_SetRenderDrawColor(render, 255, 0, 0, 127);
		SDL_RenderRect(render, &dest_rect);
		SDL_SetRenderDrawColor(render,
			buffered_color.r,
			buffered_color.g,
			buffered_color.b,
			buffered_color.a);
	}
}

bool game_object::get_collided()
{
	return collided;
}

void game_object::set_physic(bool state)
{
	this->physic = state;
}

bool game_object::get_physic()
{
	return physic;
}

void game_object::set_collide_mode(bool state)
{
	collideable = state;
}

bool game_object::get_collide_mode()
{
	return collideable;
}


bool global_check_collision(game_object* obj1, game_object* obj2)
{
	if (obj1->get_collide_mode() and obj2->get_collide_mode()) {
		auto this_rect = obj1->get_collision_box();
		auto object_rect = obj2->get_collision_box();

		bool vertical_collision = ((this_rect.y + this_rect.h) > object_rect.y) &&
			(this_rect.y < (object_rect.y + object_rect.h));

		bool horizontal_collision = ((this_rect.x + this_rect.w) > object_rect.x) &&
			(this_rect.x < (object_rect.x + object_rect.w));
		auto collided = (vertical_collision && horizontal_collision);

		return collided;
	}

}