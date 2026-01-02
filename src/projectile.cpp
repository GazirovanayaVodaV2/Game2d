#include "headers/projectile.h"

#include "player.h"
#include "dummy.h"

void projectile::interaction_impl(entity* ent)
{
	ent->damage(damage);
	exist = false;
}

projectile::projectile(std::shared_ptr<texture> txt, vec2 vel, int damage)
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
	auto txt_saved_pos = txt->get_pos();
	auto txt_saved_size = txt->get_size();

	txt->set_pos(pos);
	txt->set_size(size);

	txt->draw();

	txt->set_pos(txt_saved_pos);
	txt->set_size(txt_saved_size);
}
