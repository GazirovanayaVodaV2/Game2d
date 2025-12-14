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
protected:
	static inline const OBJECT::TYPE type = OBJECT::TYPE::TEXTURE;
public:
	texture(std::string path_);
	~texture() override;

	SDL_AppResult update(float delta_time) override;
	SDL_AppResult input(const SDL_Event* event) override;

	void change(std::string path);

	void set_pos(vec2 pos) override;
	void move_on(vec2 velocity) override;

	void set_size(vec2 size) override;

	void rotate(double angle) override;

	vec2 get_size() override;
	vec2 get_pos() override;

	float get_ratio() override;

	void set_blend(SDL_BlendMode blend_mode);
	void set_color(rgba color);
	void set_color(hex color);

	void draw() override;
	bool check_collision(std::shared_ptr<game_object> object) override;
	void clear_collision_buffer() override;

	OBJECT::TYPE get_type() override;
};

class atlas : public base {
private:
	std::map<std::string, std::shared_ptr<texture>> textures;
protected:
	static inline const OBJECT::TYPE type = OBJECT::TYPE::ATLAS;
public:
	atlas();
	~atlas() override;

	OBJECT::TYPE get_type() override;
	std::shared_ptr<texture> get(std::string name);
};
