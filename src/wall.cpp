#include "wall.h"

#include <utils.h>

void wall::save_txt_state()
{
	txt_saved_pos = txt->get_pos();
	txt_saved_size = txt->get_size();

	txt->set_pos(pos);
	txt->set_size(size);
}

void wall::restore_txt_state()
{
	txt->set_pos(txt_saved_pos);
	txt->set_size(txt_saved_size);
}

wall::wall(std::shared_ptr<texture> txt)
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
	save_txt_state();

	draw_selection();
	txt->draw();

	restore_txt_state();
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
	txt->clear_collision_buffer();
}
