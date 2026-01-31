#pragma once
#include "interactive_object.h"

class medkit : public interactive_object_base {
protected:
	texture_from_atlas txt;
	void interaction_impl(entity* ent) override {};
	bool use_impl(entity* user) override;
public:
	medkit(texture_from_atlas txt);
	~medkit();

	void draw();

	SDL_AppResult update(float delta);

	bool is_pickable() override {
		return true;
	};
};