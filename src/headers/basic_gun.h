#pragma once

#include "interactive_object.h"

class basic_gun : public interactive_object_base {
public:
	enum class SHOOT_MODE {
		ERROR,
		SEMI_AUTO,
		AUTO
	};
private:
	texture_from_atlas m_texture = nullptr;
	SHOOT_MODE m_shoot_mode = SHOOT_MODE::SEMI_AUTO;
	int m_damage = 10;
	with_default_value<int> m_mag = 10;

	bool use_impl(entity* user);

	void interaction_impl(entity* ent) override {};
public:
	basic_gun(texture_from_atlas txt, int dmg, SHOOT_MODE smode, int mag_size);
	~basic_gun() = default;

	bool is_pickable() override {
		return true;
	}

	bool can_hold_in_hands() override {
		return true;
	}

	void reload(entity* user);

	void draw();
	SDL_AppResult update(float delta);

	OBJECT::TYPE get_type() override { return OBJECT::TYPE::BASIC_GUN; }
	std::string get_name() override { return "basic_gun"; };
};