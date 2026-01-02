#pragma once
#include "game_object.h"
#include <string>
#include <map>

#include <rgba.h>

class texture :
    public game_object
{
private:
	void create_texture(std::string path_);
	void delete_texture();

	SDL_Texture* sdl_texture = nullptr;
public:
	texture(std::string path_);
	~texture() override;

	SDL_AppResult update(float delta_time) override;
	SDL_AppResult input(const SDL_Event* event) override;

	void change(std::string path);

	void move_on(vec2 velocity);

	void set_blend(SDL_BlendMode blend_mode);
	void set_color(rgba color);
	void set_color(hex color);

	void draw() override;
	bool check_collision(game_object* object) override;
	void clear_collision_buffer() override;

	OBJECT::TYPE get_type() override { return OBJECT::TYPE::TEXTURE; }
};

class atlas : public base {
private:
	std::map<std::string, std::shared_ptr<texture>> textures;
public:
	atlas();
	~atlas() override;

	std::shared_ptr<texture> get(std::string name);

	OBJECT::TYPE get_type() override {
		return OBJECT::TYPE::ATLAS;
	}
};
