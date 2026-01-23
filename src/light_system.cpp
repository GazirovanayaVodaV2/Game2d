#include "light_system.h"

#include <utils.h>

void light::source::draw_selection()
{
	auto viewport = camera::get_viewport();
	auto render = camera::get();
	if (selected) {
		auto dest_rect = pos.get_frect(size);

		dest_rect.x = (pos.x + viewport.x - 2);
		dest_rect.y = (pos.y + viewport.y - 2);

		dest_rect.w += 4;
		dest_rect.h += 4;

		SDL_Color buffered_color;
		SDL_GetRenderDrawColor(render,
			&buffered_color.r,
			&buffered_color.g,
			&buffered_color.b,
			&buffered_color.a
		);

		SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
		SDL_RenderRect(render, &dest_rect);
		SDL_SetRenderDrawColor(render,
			buffered_color.r,
			buffered_color.g,
			buffered_color.b,
			buffered_color.a);
	}
}

light::source::source(std::string path_, float radius, hex color)
{
	print::loading("Creating light source");
	print::increase_level();

	this->color = rgba(color);
	this->radius = radius;

	light_txt = std::make_unique<texture>(camera::get(), path_);
	light_txt->set_blend(SDL_BLENDMODE_ADD);
	light_txt->get_size(&size.x, &size.y);
	print::decrease_level();
	print::loaded("light source");
}

light::source::~source()
{
	print::info("Deleting light sourse");
}

SDL_AppResult light::source::update(float delta_time)
{
	return SDL_APP_CONTINUE;
}

SDL_AppResult light::source::input(const SDL_Event* event)
{
	input_if_selected(event);
	return SDL_APP_CONTINUE;
}

void light::source::set_pos(vec2 pos)
{
	this->pos = pos;
}

void light::source::move_on(vec2 velocity)
{
	this->pos += velocity;
}

void light::source::set_size(vec2 size)
{
	this->size = size;
}


vec2 light::source::get_size()
{
	return size;
}

vec2 light::source::get_pos()
{
	return pos;
}

void light::source::draw()
{
	auto render = camera::get();

	auto txt_size = size;
	auto txt_pos = pos;

	SDL_Color saved_clr;
	SDL_GetRenderDrawColor(render, &saved_clr.r, &saved_clr.g, &saved_clr.b, &saved_clr.a);

	set_size(vec2(txt_size.x * radius, txt_size.y * radius));
	set_pos(pos - (size / 2.0f));

	light_txt->set_color(color.color);

	draw_selection();
	light_txt->draw(render, camera::get_pos(), pos, size);

	set_pos(txt_pos);
	set_size(txt_size);

	SDL_SetRenderDrawColor(render, saved_clr.r, saved_clr.g, saved_clr.b, saved_clr.a);
}

void light::source::set_color(int clr)
{
	color = rgba(clr);
}

float light::source::get_radius()
{
	return radius;
}

void light::source::set_radius(float radius)
{
	this->radius = radius;
}

light::system::system()
{
	print::loading("Creating light system");
	print::increase_level();

	auto viewport = camera::get_viewport();

	ambient = std::make_shared<texture>(camera::get(), SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET,
		1000, 1000);
	ambient->set_blend(SDL_BLENDMODE_MOD);

	print::decrease_level();
	print::loaded("Light system created");
}

light::system::~system()
{
	print::info("deleting light system");
}

void light::system::add_light(std::string path, float radius, hex color)
{
	lights.push_back(std::make_shared<light::source>(path, radius, color));
}

std::shared_ptr<light::source> light::system::get_light(size_t id)
{
	return lights[id];
}

std::shared_ptr<light::source> light::system::get_last()
{
	return lights[get_count() - 1];
}

size_t light::system::get_count()
{
	return lights.size();
}

SDL_AppResult light::system::update(float delta_time)
{
	return SDL_APP_CONTINUE;
}

SDL_AppResult light::system::input(const SDL_Event* event)
{
	for (auto& light : lights) {
		light->input(event);
	}
	return SDL_APP_CONTINUE;
}

void light::system::set_pos(vec2 pos)
{
	this->pos = pos;
	for (auto& light : lights) {
		light->set_pos(pos);
	}
}

void light::system::move_on(vec2 velocity)
{
	this->pos = pos + velocity;
	for (auto& light : lights) {
		light->move_on(velocity);
	}
}



/*
For some reason avalible only white lights : (
*/
void light::system::draw()
{
	auto viewport = camera::get_viewport();
	auto render = camera::get();
	SDL_SetRenderTarget(render, ambient->get());

	SDL_SetRenderScale(render, 1000.0f / viewport.w, 1000.0f / viewport.h);

	SDL_SetRenderDrawColor(render, ambient_color.color.r, ambient_color.color.g, ambient_color.color.b, 255);
	SDL_RenderFillRect(render, NULL);

	for (auto& light : lights) {
		light->draw();
	}

	SDL_SetRenderScale(render, 1.0f, 1.0f);
	SDL_SetRenderTarget(render, NULL);
	
	SDL_FRect dst_rect = viewport;
	dst_rect.x = 0;
	dst_rect.y = 0;
	ambient->draw(render, NULL, &dst_rect);
	
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
}

void light::system::set_ambient(rgba clr)
{
	ambient_color = clr;
}

void light::system::clear()
{
	lights.clear();
}