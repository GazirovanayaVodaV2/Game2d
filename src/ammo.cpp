#include "ammo.h"

#include "window.h"

ammo::ammo(texture_from_atlas txt, int ammo_type, int count)
{
	m_texture = txt;
	size = txt->get_size();
	size = vec2(64,64);
	m_ammo_type = ammo_type;
	m_count = count;

	collideable = false;
}

void ammo::draw()
{
	m_texture->draw(camera::get(), camera::get_pos(), pos, size);
}

SDL_AppResult ammo::update(float delta)
{
	return SDL_APP_CONTINUE;
}
