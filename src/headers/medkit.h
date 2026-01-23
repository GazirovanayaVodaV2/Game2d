#pragma once
#include "interactive_object.h"

class medkit : public interactive_object_base {
protected:
	texture_from_atlas txt;
	void interaction_impl(entity* ent) override {};
	void use_impl(entity* user) override;
public:
	medkit(texture_from_atlas txt);
	~medkit() = default;

	void draw();

	SDL_AppResult update(float delta);

	bool is_pickable() override {
		return true;
	};

	OBJECT::TYPE get_type() override { return OBJECT::TYPE::MEDKIT; }
};