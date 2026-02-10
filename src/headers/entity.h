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
	std::unique_ptr<animation::player> animations;

	ENTITY::ACTION action = ENTITY::ACTION::IDLE;
	ENTITY::STATUSES statuses;
	float default_speed = 5.0f;
	vec2 movement_velocity, last_pos;
	
	with_default_value<int> hp = 100;
	
	virtual void death() {
		on_hp_change();
		hp.reset();
	};

	virtual void on_hp_change() {};
public:
	~entity() = default;

	virtual void set_hp(int hp) {
		this->hp = hp;
		on_hp_change();
	};
	virtual void damage(int damage) {
		this->hp -= damage;
		on_hp_change();
		if (this->hp <= 0) {
			death();
		}
	};
	virtual void heal(int hp) {
		this->hp += hp;
		if (this->hp > this->hp.get_default()) {
			this->hp.reset();
		}
		on_hp_change();
	};
	int get_hp() {
		return hp;
	};
};

 