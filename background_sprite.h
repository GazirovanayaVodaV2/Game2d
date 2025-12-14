#pragma once
#include "wall.h"

class background_sprite : public wall {
protected:
	bool collideable = false;

	static inline const OBJECT::TYPE type = OBJECT::TYPE::BACKGROUND_SPRITE;
public:
	background_sprite(std::shared_ptr<texture> txt) : wall(txt) {};
	bool check_collision(std::shared_ptr<game_object> object) override { return false; };
	void set_collide_mode(bool state) = delete;
	bool get_collide_mode() { return false; };
};