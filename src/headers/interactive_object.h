#pragma once

#include <memory>
#include <functional>
#include <vector>

#include "base.h"
#include "game_object.h"
#include "texture.h"
#include "entity.h"

class interactive_object_base : public game_object {
private:
	bool entity_nearby = false;

protected:
	const float use_dist = 100.0f;

	static inline const OBJECT::TYPE type = OBJECT::TYPE::INTERACTIVE_OBJECT;

	virtual void interaction_impl(std::shared_ptr<entity> ent) = 0;
	void interact(std::shared_ptr<entity> ent);
	std::vector<std::function<void(std::shared_ptr<entity>)>> custom_interactions;

	virtual void use_impl(std::shared_ptr<entity> user) = 0;
public:
	interactive_object_base() = default;

	~interactive_object_base() = default;

	bool check_collision(std::shared_ptr<game_object> object) override;
	void clear_collision_buffer() override;

	interactive_object_base& add_interaction(std::function<void(std::shared_ptr<entity>)> inter);

	void use(std::shared_ptr<entity> user);

	//Shitty shit
	void set_pos(vec2 pos) override {
		this->pos = pos;
	};
	void move_on(vec2 velocity) override {
		this->pos += velocity;
	};
	void set_size(vec2 size) override {
		this->size = size;
	};
	void rotate(double angle) override {
		this->angle = angle;
	};
	vec2 get_size() override {
		return size;
	};
	vec2 get_pos() override {
		return pos;
	};
	float get_ratio() override {
		return size.x() / size.y();
	};
};