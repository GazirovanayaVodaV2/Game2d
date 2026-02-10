#include <format>
#include <fstream>

#include "texture.h"

#include "SDL3_image/SDL_image.h"
#include "Utils.h"

#include "nlohmann/json.hpp"

void texture::create_texture(SDL_Renderer* render, std::string path_)
{
	if (!txt) {
		txt = IMG_LoadTexture(render, path(path_).c_str());
		if (!txt) {
			print::error(path_, SDL_GetError());
		}

	}
	else {
		print::warning("Texture already exist!", path_);
	}
}

void texture::delete_texture()
{
	SDL_DestroyTexture(txt);
	txt = nullptr;
}

texture::texture(SDL_Renderer* render, std::string path_)
{
	create_texture(render, path_);
}

texture::texture(SDL_Renderer* render, SDL_PixelFormat format, SDL_TextureAccess txt_access, int w, int h)
{
	txt = SDL_CreateTexture(render, format, txt_access, w, h);
	if (!txt) {
		print::error("Failed to create texture!");
	}
}

texture::~texture()
{
	delete_texture();
	print::info("Deleting texture");
}

void texture::change(SDL_Renderer* render, std::string path_)
{

	print::loading(std::format("Changing texture: {}", path_));

	delete_texture();
	create_texture(render, path_);

	print::loaded();

}

void texture::set_blend(SDL_BlendMode blend_mode)
{
	SDL_SetTextureBlendMode(txt, blend_mode);
}

void texture::set_color(rgba color)
{
	auto sdl_clr = color.color;
	SDL_SetTextureColorMod(txt, sdl_clr.r, sdl_clr.g, sdl_clr.b);
}

void texture::set_color(hex color)
{
	set_color(rgba(color));
}

void texture::set_alpha(uint8_t alpha)
{
	SDL_SetTextureAlphaMod(txt, alpha);
}

vec2 texture::get_size()
{
	vec2 size;
	get_size(&size.x, &size.y);
	return size;
}

void texture::get_size(float* w, float* h)
{
	SDL_GetTextureSize(txt, w, h);
}

void texture::draw(SDL_Renderer* render, vec2 pos, vec2 size)
{
	auto frect = pos.get_frect(size);
	draw(render, &frect);
}

void texture::draw(SDL_Renderer* render, SDL_FRect* dst)
{
	draw(render, NULL, dst);
}

void texture::draw(SDL_Renderer* render, SDL_FRect* src, SDL_FRect* dst)
{
	SDL_RenderTexture(render, txt, src, dst);
}

void texture::draw_rotated(SDL_Renderer* render, vec2 pos, vec2 size, float angle, SDL_FlipMode flip)
{
	auto frect = pos.get_frect(size);
	draw_rotated(render, &frect, angle, flip);
}

void texture::draw_rotated(SDL_Renderer* render, SDL_FRect* dst, float angle, SDL_FlipMode flip)
{
	draw_rotated(render, NULL, dst, angle, flip);
}

void texture::draw_rotated(SDL_Renderer* render, SDL_FRect* src, SDL_FRect* dst, float angle, SDL_FlipMode flip)
{
	SDL_RenderTextureRotated(render, txt, src, dst, angle, NULL, flip);
}

void texture::draw(SDL_Renderer* render, vec2 cam_pos, vec2 pos, vec2 size)
{
	auto frect = pos.get_frect(size);
	draw(render, cam_pos, &frect);
}

void texture::draw(SDL_Renderer* render, vec2 cam_pos, SDL_FRect* dst)
{
	draw(render, cam_pos, NULL, dst);
}

void texture::draw(SDL_Renderer* render, vec2 cam_pos, SDL_FRect* src, SDL_FRect* dst)
{
	auto res_dst = SDL_FRect{
		dst->x + cam_pos.x,
		dst->y + cam_pos.y,
		dst->w,
		dst->h
	};
	draw(render, src, &res_dst);
}

void texture::draw_rotated(SDL_Renderer* render, vec2 cam_pos, vec2 pos, vec2 size, float angle, SDL_FlipMode flip)
{
	auto frect = pos.get_frect(size);
	draw_rotated(render, cam_pos, &frect, angle, flip);
}

void texture::draw_rotated(SDL_Renderer* render, vec2 cam_pos, SDL_FRect* dst, float angle, SDL_FlipMode flip)
{
	draw_rotated(render, cam_pos, NULL, dst, angle, flip);
}

void texture::draw_rotated(SDL_Renderer* render, vec2 cam_pos, SDL_FRect* src, SDL_FRect* dst, float angle, SDL_FlipMode flip)
{
	auto res_dst = SDL_FRect{
		dst->x + cam_pos.x,
		dst->y + cam_pos.y,
		dst->w,
		dst->h
	};
	draw_rotated(render, src, &res_dst, angle, flip);
}

atlas::atlas(SDL_Renderer* render)
{
	print::loading("Loading textures");
	print::increase_level();

	using json = nlohmann::json;
	std::ifstream file(path("textures/textures.json"));
	auto json_textures = json::parse(file);
	file.close();

	int count = json_textures.size();
	int i = 0;
	
	for (auto& [key, value] : json_textures.items()) {
		print::loading(std::format("Loading texture {}/{}: {}", ++i, count, key));
		textures[key] = std::make_unique<texture>(render, "textures/" + std::string(value));
	}

	print::decrease_level();
	print::loaded("Textures loaded");
}

atlas::~atlas()
{
	print::info("Deleting texture context");
	print::increase_level();
	int i = 0, count = textures.size();
	for (auto& txt : textures) {
		print::info(std::format("{}/{}: {}", ++i, count, txt.first));
		txt.second.reset();
	}
	textures.clear();

	print::decrease_level();
	print::info("Atlas deleted!");
}


texture_from_atlas atlas::get(std::string name)
{
	return textures[name].get();
}
