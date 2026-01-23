#include "headers/projectile.h"

#include "player.h"
#include "dummy.h"

void projectile::interaction_impl(entity* ent)
{
	ent->damage(damage);
	exist = false;
}

projectile::projectile(texture_from_atlas txt, vec2 vel, int damage)
{
	this->txt = txt;
	this->vel = vel;
	this->damage = damage;
	this->size = txt->get_size() / 4;
}

SDL_AppResult projectile::update(float delta_time)
{
	move_on(fps::synch<vec2>(vel));

	current_life_time += delta_time / pow(10, 9);

	if (current_life_time > 20) {
		exist = false;
	}

	return SDL_APP_CONTINUE;
}

bool projectile::check_collision(game_object* object)
{
	auto res = interactive_object_base::check_collision(object);

	if (res && (object->get_type() == OBJECT::TYPE::WALL)) {
		exist = false;
	}

	return res;
}

void projectile::draw()
{
	txt->draw(camera::get(), camera::get_pos(), pos, size);
}
