#include "game_object.h"
#include "window.h"

void game_object::draw_selection()
{
	auto viewport = camera::get_viewport();
	auto render = camera::get();
	if (selected) {
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
