#pragma once

#include <memory>

#include "game_object.h"
#include "texture.h"

class wall :
    public game_object
{
protected:
	texture_from_atlas txt;
	bool physic = true;
public:
	wall(texture_from_atlas txt);
	~wall() override;

	SDL_AppResult update(float delta_time) override;
	SDL_AppResult input(const SDL_Event* event) override;

	void draw() override;
	bool check_collision(game_object* object) override;
	void clear_collision_buffer() override;
};