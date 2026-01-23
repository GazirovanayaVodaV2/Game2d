#pragma once
#include "interactive_object.h"

class projectile : public interactive_object_base {
protected:
	vec2 vel;
	int damage = 0;
	float current_life_time = 0;

	texture_from_atlas txt;

	void interaction_impl(entity* ent) override;
	void use_impl(entity* user) override {};
public:
	projectile() = default;
	projectile(texture_from_atlas txt, vec2 vel, int damage = 100);

	~projectile() = default;

	SDL_AppResult update(float delta_time) override;
	bool check_collision(game_object* object);

	void draw() override;

	OBJECT::TYPE get_type() override { return OBJECT::TYPE::PROJECTILE; }
};