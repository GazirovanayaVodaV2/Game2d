#include "medkit.h"

void medkit::use_impl(entity* user)
{
	user->heal(20);
	exist = false;
}

medkit::medkit(std::shared_ptr<atlas> atl)
{
	txt = atl->get("medkit");
	size = txt->get_size();
	collideable = false;
}

void medkit::draw()
{
	auto saved_pos = txt->get_pos();
	txt->set_pos(pos);
	txt->draw();
	txt->set_pos(saved_pos);
}

SDL_AppResult medkit::update(float delta)
{
	return SDL_APP_CONTINUE;
}
