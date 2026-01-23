#pragma once
#include "wall.h"

class background_sprite : public wall {
protected:
	bool collideable = false;
public:
	background_sprite(texture_from_atlas txt) : wall(txt) {};
	bool check_collision(game_object* object) override { return false; };
	void set_collide_mode(bool state) = delete;
	bool get_collide_mode() { return false; };

	OBJECT::TYPE get_type() override {
		return OBJECT::TYPE::BACKGROUND_SPRITE;
	};
};