#pragma once

#include <memory>
#include <functional>
#include <vector>

#include "base.h"
#include "game_object.h"
#include "texture.h"
#include "entity.h"

class interactive_object_base : public game_object {
protected:
	bool entity_nearby = false;
	bool in_inventory_ = false;
	const float use_dist = 100.0f;

	virtual void interaction_impl(entity* ent) = 0;
	void interact(entity* ent);
	std::vector<std::function<void(entity*)>> custom_interactions;

	virtual bool use_impl(entity* user) = 0;

	virtual std::string send_msg_to_player() { return "press to E pick up!"; };
public:
	interactive_object_base() = default;

	~interactive_object_base() = default;

	bool check_collision(game_object* object) override;
	void clear_collision_buffer() override;

	interactive_object_base& add_interaction(std::function<void(entity*)> inter);

	bool use(entity* user);

	SDL_AppResult input(const SDL_Event* ev) override { return SDL_APP_CONTINUE; };

	virtual void pick_up();
	virtual bool is_pickable() {
		return false;
	}

	virtual bool can_hold_in_hands() {
		return false;
	}

	bool in_inventory();
	virtual std::string get_name() { return ""; };
};