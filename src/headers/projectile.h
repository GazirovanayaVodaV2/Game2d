#pragma once
#include "interactive_object.h"

class projectile : public interactive_object_base {
protected:
	vec2 vel;
	int damage = 0;
	float current_life_time = 0;
	const float bullet_size = 6;

	texture_from_atlas txt;

	void interaction_impl(entity* ent) override;
	bool use_impl(entity* user) override { return false; };
public:
	projectile() = default;
	projectile(texture_from_atlas txt, vec2 vel, int damage = 100);

	~projectile();

	SDL_AppResult update(float delta_time) override;
	bool check_collision(game_object* object);

	void draw() override;
};