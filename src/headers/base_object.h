#pragma once
#include <base.h>
#include <memory>

#include <SDL3/SDL.h>

#include <vector.h>

enum class OBJECT_DIRECTION {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class base_object : public base
{
protected:
	static inline const OBJECT::TYPE type = OBJECT::TYPE::BASE_OBJECT;

	vec2 pos, size;
	double angle = 0.0f;
	OBJECT_DIRECTION dir = OBJECT_DIRECTION::LEFT;

	bool selected = false;

	void input_if_selected(const SDL_Event* event);
public:
	~base_object() = default;
	
	void select() {
		selected = true;
	}
	void reject() {
		selected = false;
	}

	bool is_selected() {
		return selected;
	}

	virtual SDL_AppResult update(float delta_time) = 0;
	virtual SDL_AppResult input(const SDL_Event* event) = 0;
	
	virtual void set_pos(vec2 pos) {
		this->pos = pos;
	};
	virtual void move_on(vec2 velocity) {
		this->pos += velocity;
	};

	virtual void set_size(vec2 size) {
		this->size = size;
	};

	virtual void rotate(double angle) {
		this->angle = angle;
	};

	virtual vec2 get_size() {
		return size;
	};
	virtual vec2 get_pos() {
		return pos;
	};

	double get_angle() {
		return angle;
	}

	virtual float get_ratio() {
		return size.x() / size.y();
	};

	bool in(vec2 pos) {
		return (pos.x() > this->pos.x()) and (pos.x() < (this->pos.x() + this->size.x()))
			and (pos.y() > this->pos.y()) and (pos.y() < (this->pos.y() + this->size.y()));
	}
};

 