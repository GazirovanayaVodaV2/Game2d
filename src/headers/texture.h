#pragma once

#include <string>
#include <functional>

#include "SDL3/SDL.h"
#include "rgba.h"
#include "vector.h"

class texture {
private:
	SDL_Texture* txt = nullptr;

	void create_texture(SDL_Renderer* render, std::string path_);
	void delete_texture();
public:
	texture() = default;
	texture(SDL_Renderer* render, std::string path_);
	texture(SDL_Renderer* render, SDL_PixelFormat format, SDL_TextureAccess txt_access, int w, int h);

	texture(const texture&) = delete;
	texture& operator=(const texture&) = delete;

	~texture();

	void change(SDL_Renderer* render, std::string path);
	void set_blend(SDL_BlendMode blend_mode);
	void set_color(rgba color);
	void set_color(hex color);

	void set_alpha(uint8_t alpha);

	vec2 get_size();
	void get_size(float* w, float* h);
	
	void draw(SDL_Renderer* render, vec2 pos, vec2 size);
	void draw(SDL_Renderer* render, SDL_FRect* dst);
	void draw(SDL_Renderer* render, SDL_FRect* src, SDL_FRect* dst);
	
	void draw_rotated(SDL_Renderer* render, vec2 pos, vec2 size, float angle, SDL_FlipMode flip = SDL_FLIP_NONE);
	void draw_rotated(SDL_Renderer* render, SDL_FRect* dst, float angle, SDL_FlipMode flip = SDL_FLIP_NONE);
	void draw_rotated(SDL_Renderer* render, SDL_FRect* src, SDL_FRect* dst, float angle, SDL_FlipMode flip = SDL_FLIP_NONE);

	void draw(SDL_Renderer* render, vec2 cam_pos, vec2 pos, vec2 size);
	void draw(SDL_Renderer* render, vec2 cam_pos, SDL_FRect* dst);
	void draw(SDL_Renderer* render, vec2 cam_pos, SDL_FRect* src, SDL_FRect* dst);
	
	void draw_rotated(SDL_Renderer* render, vec2 cam_pos, vec2 pos, vec2 size, float angle, SDL_FlipMode flip = SDL_FLIP_NONE);
	void draw_rotated(SDL_Renderer* render, vec2 cam_pos, SDL_FRect* dst, float angle, SDL_FlipMode flip = SDL_FLIP_NONE);
	void draw_rotated(SDL_Renderer* render, vec2 cam_pos, SDL_FRect* src, SDL_FRect* dst, float angle, SDL_FlipMode flip = SDL_FLIP_NONE);

	SDL_Texture* get() {
		return txt;
	}
};

typedef texture* texture_from_atlas;
class atlas {
private:
	std::map<std::string, std::unique_ptr<texture>> textures;
public:
	atlas(SDL_Renderer* render);
	~atlas();

	texture_from_atlas get(std::string name);
};
