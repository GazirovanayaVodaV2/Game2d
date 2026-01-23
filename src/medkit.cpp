#include "medkit.h"

#include "window.h"

void medkit::use_impl(entity* user)
{
	user->heal(20);
	exist = false;
}

medkit::medkit(texture_from_atlas txt)
{
	this->txt = txt;
	size = txt->get_size();
	collideable = false;
}

void medkit::draw()
{
	txt->draw(camera::get(), camera::get_pos(), pos, size);
}

SDL_AppResult medkit::update(float delta)
{
	return SDL_APP_CONTINUE;
}
