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

	OBJECT::TYPE get_type() override { return OBJECT::TYPE::GAME_OBJECT; }
};

class NULL_OBJECT : public game_object {
public:
	NULL_OBJECT() = default;
	 ~NULL_OBJECT() = default;

	 SDL_AppResult update(float delta_time) override {
		 return SDL_APP_CONTINUE;
	 };
	 SDL_AppResult input(const SDL_Event* event) override {
		 return SDL_APP_CONTINUE;
	 };

	 void set_pos(vec2 pos) {};
	 void move_on(vec2 velocity) {};

	 void set_size(vec2 size) {};
	 void rotate(double angle)  {};

	 void draw() override {};
	 virtual void draw_debug() override {};
	 bool check_collision(game_object* object) override { return false; };
	 void clear_collision_buffer() override {};

	 bool get_collided() {
		 return false;
	 };
	 void set_physic(bool state) {};
	 bool get_physic() {
		 return false;
	 };

	 OBJECT::TYPE get_type() override { return OBJECT::TYPE::NULL_OBJECT; }
};


bool global_check_collision(game_object* obj1, game_object* obj2);