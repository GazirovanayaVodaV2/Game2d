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
	
	 void set_pos(vec2 pos) {
		this->pos = pos;
	};
	 void move_on(vec2 velocity) {
		this->pos += velocity;
	};

	 void set_size(vec2 size) {
		this->size = size;
	};

	 void rotate(double angle) {
		this->angle = angle;
	};

	 vec2 get_size() {
		return size;
	};
	 vec2 get_pos() {
		return pos;
	};

	 float get_distance(base_object* obj);
	 float get_sqr_distance(base_object* obj);

	 float get_size_radius();

	double get_angle() {
		return angle;
	}

	 float get_ratio() {
		return size.x / size.y;
	};

	bool in(vec2 pos) {
		return (pos.x > this->pos.x) and (pos.x < (this->pos.x + this->size.x))
			and (pos.y > this->pos.y) and (pos.y < (this->pos.y + this->size.y));
	}

	vec2 get_direction() {
		auto dir = vec2(cosf((float)angle),
			sinf((float)angle));
		if (this->dir == OBJECT_DIRECTION::RIGHT) dir.x *= -1;

		return dir;
	}

	OBJECT_DIRECTION get_direction_enum() {
		return this->dir;
	}
};


 