#pragma once

#include <memory>

#include "game_object.h"
#include "texture.h"

class wall :
    public game_object
{
protected:
	std::shared_ptr<texture> txt;

	vec2 txt_saved_pos, txt_saved_size;

	void save_txt_state();
	void restore_txt_state();
	bool physic = true;
public:
	wall(std::shared_ptr<texture> txt);
	~wall() override;

	SDL_AppResult update(float delta_time) override;
	SDL_AppResult input(const SDL_Event* event) override;

	void draw() override;
	bool check_collision(game_object* object) override;
	void clear_collision_buffer() override;

	OBJECT::TYPE get_type() override { return OBJECT::TYPE::WALL; }
};