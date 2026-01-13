#pragma once
#include "interactive_object.h"

class medkit : public interactive_object_base {
protected:
	std::shared_ptr<texture> txt;

	void interaction_impl(entity* ent) override {};
	void use_impl(entity* user) override;
public:
	medkit(std::shared_ptr<atlas> atl);
	~medkit() = default;

	void draw();

	SDL_AppResult update(float delta);

	bool is_pickable() override {
		return true;
	};

	OBJECT::TYPE get_type() override { return OBJECT::TYPE::MEDKIT; }
};