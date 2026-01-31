#pragma once

#include "entity.h"
#include "GuiLib/gui_elements.h"

class dummy : public entity {
private:
	gui::text_box* hp_box = nullptr;
	texture_from_atlas txt;

private:
	void update_hp_box();
protected:
	void death() override;

	void on_hp_change() override {
		update_hp_box();
	}
public:
	dummy(texture_from_atlas atl, int hp = 100);
	~dummy();

	void draw() override;

	SDL_AppResult update(float delta_time) override;
	SDL_AppResult input(const SDL_Event* event) override { return SDL_APP_CONTINUE; };

	bool check_collision(game_object* object) override;
	void clear_collision_buffer() override;
};