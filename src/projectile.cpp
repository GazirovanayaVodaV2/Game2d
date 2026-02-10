#include "headers/projectile.h"

#include "player.h"
#include "dummy.h"
#include "wall.h"

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
	this->size = txt->get_size() / bullet_size;
}

projectile::~projectile()
{
	print::info("Projectile deleted!");
}

SDL_AppResult projectile::update(float delta_time)
{
	move_on(fps::synch<vec2>(vel));

	current_life_time += delta_time / powf(10, 9);

	if (current_life_time > 20) {
		exist = false;
	}

	clear_collision_buffer();

	return SDL_APP_CONTINUE;
}

bool projectile::check_collision(game_object* object)
{
	auto res = interactive_object_base::check_collision(object);

	if (res && cmp_type(*object, wall)) {
		exist = false;
	}

	return res;
}

void projectile::draw()
{
	auto draw_size = size * bullet_size;

	txt->draw(camera::get(), camera::get_pos(), pos - draw_size / 2, draw_size);
}
