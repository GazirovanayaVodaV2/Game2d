#pragma once
#include "base.h"

#include "game_object.h"

#include <animation.h>

namespace ENTITY {
	enum class ACTION {
		IDLE,
		MOVING_BY_KEYBOARD,
		WALKING,
		FALLING,
		JUMPING
	};

	struct STATUSES {
		bool can_jump = false;
		bool on_floor = false;

	};
}

class entity :
    public game_object
{
protected:
	std::shared_ptr<animation::player> animations;
	static inline const OBJECT::TYPE type = OBJECT::TYPE::ENTITY;

	ENTITY::ACTION action = ENTITY::ACTION::IDLE;
	ENTITY::STATUSES statuses;
	float default_speed = 5.0f;
	vec2 movement_velocity, last_pos;
	
	with_default_value<int> hp = 100;
	
	virtual void death() {
		hp.reset();
	};
public:
	~entity() = default;

	virtual void set_hp(int hp) {
		this->hp = hp;
	};
	virtual void damage(int damage) {
		this->hp -= hp;

		if (this->hp < 0) {
			death();
		}
	};
	virtual void heal(int hp) {
		this->hp += hp;
	};
	virtual int get_hp() {
		return hp;
	};
};

class player : public entity {
private:
	bool fly = false;
	bool noclip = false;
protected:
	OBJECT::TYPE type = OBJECT::TYPE::PLAYER;
	float default_speed = 10.0f;

	void death() override {
		entity::death();
	};
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
	bool check_collision(std::shared_ptr<game_object> object) override;
	void clear_collision_buffer() override;

	OBJECT::TYPE get_type() override;

	void set_hp(int hp) override;
	void damage(int damage) override;
	void heal(int hp) override;
	int get_hp() override;
};

