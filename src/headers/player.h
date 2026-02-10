#pragma once

#include "entity.h"

#include "inventory.h"

class player : public entity {
private:
	bool fly = false;
	bool noclip = false;

	inventory::inventory invent;
	std::string object_msg;
protected:
	float default_speed = 10.0f;

	void death() override;
public:
	player(SDL_Renderer* render, std::string animation_config);
	~player() override;

	SDL_AppResult update(float delta_time) override;
	SDL_AppResult input(const SDL_Event* event) override;

	void set_pos(vec2 pos);
	void move_on(vec2 velocity);

	void set_size(vec2 size);

	void rotate(double angle);

	vec2 get_size();
	vec2 get_pos();

	float get_ratio();

	void draw() override;
	bool check_collision(game_object* object) override;
	void clear_collision_buffer() override;

	void set_hp(int hp) override;
	void damage(int damage) override;
	void heal(int hp) override;
	int get_hp();

	void get_msg_from_int_obj(std::string msg);

	inventory::inventory& get_inventory();
};

