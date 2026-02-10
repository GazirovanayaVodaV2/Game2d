#pragma once
#include "base_object.h"

#include <list>

class game_object : public base_object {
protected:
	
	std::list<game_object*> collided_objects;

	bool collided = false;
	bool collideable = true;
	bool physic = false;
	
	void draw_selection();
public:
	bool exist = true;

	virtual ~game_object() = default;

	virtual void draw() = 0;
	virtual void draw_debug();
	virtual bool check_collision(game_object* object) = 0;
	virtual void clear_collision_buffer() = 0;

	bool get_collided();
	void set_physic(bool state);
	bool get_physic();

	void set_collide_mode(bool state);
	bool get_collide_mode();
	virtual SDL_FRect get_collision_box() {
		return pos.get_frect(size);
	}
};


bool global_check_collision(game_object* obj1, game_object* obj2);
bool force_global_check_collision(game_object* obj1, game_object* obj2);