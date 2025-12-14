#pragma once
#include "base_object.h"

#include <list>

class game_object : public base_object {
protected:
	static inline const OBJECT::TYPE type = OBJECT::TYPE::GAME_OBJECT;
	
	std::list<std::shared_ptr<game_object>> collided_objects;

	bool collided = false;
	bool collideable = true;
	bool physic = false;

	void draw_selection();
public:
	virtual ~game_object() = default;

	virtual void draw() = 0;
	virtual bool check_collision(std::shared_ptr<game_object> object) = 0;
	virtual void clear_collision_buffer() = 0;

	bool get_collided();
	void set_physic(bool state);
	bool get_physic();

	void set_collide_mode(bool state);
	bool get_collide_mode();
};

class NULL_OBJECT : public game_object {
protected:
	static inline const OBJECT::TYPE type = OBJECT::TYPE::NULL_OBJECT;

public:
	NULL_OBJECT() = default;
	 ~NULL_OBJECT() = default;

	 SDL_AppResult update(float delta_time) override {
		 return SDL_APP_CONTINUE;
	 };
	 SDL_AppResult input(const SDL_Event* event) override {
		 return SDL_APP_CONTINUE;
	 };

	 void set_pos(vec2 pos) override {};
	 void move_on(vec2 velocity) override {};

	 void set_size(vec2 size) override {};
	 void rotate(double angle)  override {};

	 vec2 get_size() override {
		 return size;
	 };
	 vec2 get_pos() override {
		 return pos;
	 };

	 double get_angle() {
		 return angle;
	 }

	 float get_ratio() override {
		 return size.x() / size.y();
	 };


	 void draw() override {};
	 bool check_collision(std::shared_ptr<game_object> object) override { return false; };
	 void clear_collision_buffer() override {};

	 bool get_collided() {
		 return false;
	 };
	 void set_physic(bool state) {};
	 bool get_physic() {
		 return false;
	 };

	 OBJECT::TYPE get_type() override {
		 return type;
	 }
};