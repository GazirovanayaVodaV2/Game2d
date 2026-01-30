#include "medkit.h"

#include "window.h"

bool medkit::use_impl(entity* user)
{
	user->heal(20);
	exist = false;
	return true;
}

medkit::medkit(texture_from_atlas txt)
{
	this->txt = txt;
	size = txt->get_size();
	collideable = false;
}

medkit::~medkit()
{
	print::info("Medkit deleted!");
}

void medkit::draw()
{
	txt->draw(camera::get(), camera::get_pos(), pos, size);
}

SDL_AppResult medkit::update(float delta)
{
	return SDL_APP_CONTINUE;
}
