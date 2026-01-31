#include "basic_gun.h"
#include "ammo.h"

#include "map.h"


basic_gun::basic_gun(texture_from_atlas txt, int dmg, SHOOT_MODE smode, int mag_size)
{
	m_texture = txt;

	m_damage = dmg;
	m_shoot_mode = smode;

	m_mag.set_new_defaults(mag_size);

	size = txt->get_size();
	collideable = false;
}

void basic_gun::reload(entity* user)
{
	if (auto* pl = dynamic_cast<player*>(user)) {
		auto& inv = pl->get_inventory();

		auto it = inv.find_item(typeid(ammo));
		if (it) {
			auto ammo_it = (ammo*)it;
			int diff = m_mag.get_default() - *m_mag;
			m_mag += diff;
			ammo_it->set_count(ammo_it->get_count() - diff);
			if (ammo_it->get_count() <= 0) {
				inv.remove_item(typeid(ammo));
			}
		}
	}
	else {
		m_mag.reset();
	}
}
void basic_gun::draw()
{
	m_texture->draw(camera::get(), camera::get_pos(), pos, size);
}

SDL_AppResult basic_gun::update(float delta)
{
	return SDL_APP_CONTINUE;
}


bool basic_gun::use_impl(entity* user)
{
	if (m_mag <= 0) return false;

	auto level = level_manager::get();
	if (level) {
		float bullet_speed = 5.0f;

		auto user_dir = user->get_direction_enum();
		vec2 bullet_vel = (user_dir == OBJECT_DIRECTION::RIGHT) ? vec2(bullet_speed, 0.0f) : vec2(-bullet_speed, 0.0f);
		vec2 bullet_spawn_pos = (user_dir == OBJECT_DIRECTION::RIGHT) ? vec2(80.0f, 0.0f) : vec2(-32.0f, 0.0f);
		bullet_spawn_pos += user->get_pos();
		level->add_bullet(m_damage, bullet_spawn_pos, bullet_speed, bullet_vel);

		m_mag -= 1;

		return true;
	}

	return false;
}
