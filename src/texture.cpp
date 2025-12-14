#include "texture.h"
#include <SDL3_image/SDL_image.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <SDL3/SDL.h>
#include <format>
#include <utils.h>
#include <rgba.h>

#include "window.h"

void texture::create_texture(std::string path_)
{
	print::loading(path_);
	sdl_texture = IMG_LoadTexture(camera::get() , path(path_).c_str());
	if (!sdl_texture) {
		throw std::format("Something wrong with texture. Path: {} Log: {} Error Code: {}",
			path(path_), SDL_GetError(), static_cast<int>(SDL_APP_FAILURE)).c_str();
	}
	float w, h;
	SDL_GetTextureSize(sdl_texture, &w, &h);
	size = vec2(w, h);

	print::loaded();
}

void texture::delete_texture()
{
	print::info("Deleting texture");
	SDL_DestroyTexture(sdl_texture);
}

texture::texture(std::string path_)
{
	create_texture(path_);
}

texture::~texture()
{
	delete_texture();
}

SDL_AppResult texture::update(float delta_time)
{
	return SDL_APP_CONTINUE;
}

SDL_AppResult texture::input(const SDL_Event* event)
{
	input_if_selected(event);
	return SDL_APP_CONTINUE;
}

void texture::change(std::string path)
{
	print::loading(std::format("Changing texture: {}", path));

	delete_texture();
	create_texture(path);

	print::loaded();
}

void texture::set_pos(vec2 pos)
{
	this->pos = pos;
}

void texture::move_on(vec2 velocity)
{
	if (velocity.x() < 0.0f) {
		dir = OBJECT_DIRECTION::LEFT;
	}
	else if (velocity.x() > 0.0f) {
		dir = OBJECT_DIRECTION::RIGHT;
	}

	set_pos(get_pos() + velocity);
}

void texture::set_size(vec2 size)
{
	this->size = size;
}

void texture::rotate(double angle)
{
	this->angle = angle;
}

vec2 texture::get_size()
{
	return size;
}

vec2 texture::get_pos()
{
	return pos;
}

float texture::get_ratio()
{
	return size.x() / size.y();
}

void texture::set_blend(SDL_BlendMode blend_mode)
{
	SDL_SetTextureBlendMode(sdl_texture, blend_mode);
}

void texture::set_color(rgba color)
{
	auto sdl_clr = color.color;
	SDL_SetTextureColorMod(sdl_texture, sdl_clr.r, sdl_clr.g, sdl_clr.b);
}

void texture::set_color(hex color)
{
	set_color(rgba(color));
}

void texture::draw()
{
	auto viewport = camera::get_viewport();
	auto render = camera::get();

	auto dest_rect = pos.get_frect(size);

	dest_rect.x = (pos.x() + viewport.x);
	dest_rect.y = (pos.y() + viewport.y);

	auto flip = SDL_FLIP_NONE;
	if (dir == OBJECT_DIRECTION::RIGHT) {
		flip = SDL_FLIP_HORIZONTAL;
	}

	draw_selection();
	SDL_RenderTextureRotated(render, sdl_texture, NULL, &dest_rect, angle, NULL, flip);
}

bool texture::check_collision(std::shared_ptr<game_object> object)
{
	//TODO
	if (physic) {
		auto this_rect = pos.get_frect(size);
		auto object_rect = object->get_pos().get_frect(object->get_size());

		bool vertical_collision = ((this_rect.y + this_rect.h) > object_rect.y) &&
			(this_rect.y < (object_rect.y + object_rect.h));

		bool horizontal_collision = ((this_rect.x + this_rect.w) > object_rect.x) &&
			(this_rect.x < (object_rect.x + object_rect.w));
		collided = (vertical_collision && horizontal_collision);

		if (collided) {
			collided_objects.push_back(object);
		}

		return collided;
	} return false;
}

void texture::clear_collision_buffer()
{
	collided_objects.clear();
}

OBJECT::TYPE texture::get_type()
{
	return type;
}

atlas::atlas()
{
	print::loading("Loading textures");
	print::increase_level();

	using json = nlohmann::json;
	std::ifstream file(path("textures\\textures.json"));
	auto json_textures = json::parse(file);
	file.close();

	std::vector<std::string> keys;
	std::map<std::string, std::string> paths;

	for (auto& element : json_textures.items()) {
		keys.push_back(element.key());
	}

	for (auto& key : keys) {
		paths[key] = json_textures.at(key);
	}

	for (auto& [key, path] : paths) {
		textures[key] = std::make_shared<texture>("textures\\"+path);
	}

	print::decrease_level();
	print::loaded("Textures loaded");
}

atlas::~atlas()
{
	print::info("Deleting texture context");
}

OBJECT::TYPE atlas::get_type()
{
	return type;
}

std::shared_ptr<texture> atlas::get(std::string name)
{
	return textures[name];
}
