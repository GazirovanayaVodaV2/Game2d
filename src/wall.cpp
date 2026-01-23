#include "wall.h"
#include "window.h"
#include "utils.h"

wall::wall(texture_from_atlas txt)
{
	print::loading("Spawning wall");
	print::increase_level();

	this->txt = txt;

	print::decrease_level();
	print::loaded("Wall spawned");
}

wall::~wall()
{
	print::info("Deleting wall");
}

SDL_AppResult wall::update(float delta_time)
{
	return SDL_APP_CONTINUE;
}

SDL_AppResult wall::input(const SDL_Event* event)
{
	input_if_selected(event);
	return SDL_APP_CONTINUE;
}

void wall::draw()
{
	draw_selection();
	txt->draw(camera::get(), camera::get_pos(), pos, size);
}

bool wall::check_collision(game_object* object)
{
	if (physic) {
		collided = global_check_collision(this, object);

		if (collided) {
			collided_objects.push_back(object);
		}

		return collided;
	} return false;
}

void wall::clear_collision_buffer()
{
	collided_objects.clear();
}
