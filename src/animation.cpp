#include "animation.h"
#include <utils.h>

#include <typeinfo>
#include <nlohmann/json.hpp>
#include <fstream>


animation::frame::frame(SDL_Renderer* render, std::string path, int time)
{
	print::loading("Creating frame");
	print::increase_level();

	msec = time;
	txt = std::make_shared<texture>(path);
	this->path = path;

	size = txt->get_size();

	print::decrease_level();
	print::loaded();
}

animation::frame::~frame()
{
	print::info("Deleting frame. Path: " + path);
}

SDL_AppResult animation::frame::update(float delta_time)
{
	return txt->update(delta_time);
}

SDL_AppResult animation::frame::input(const SDL_Event* event)
{
	return txt->input(event);
}

void animation::frame::set_pos(vec2 pos)
{
	txt->set_pos(pos);
}

void animation::frame::move_on(vec2 velocity)
{
	txt->move_on(velocity);
}

void animation::frame::set_size(vec2 size)
{
	txt->set_size(size);
}

void animation::frame::rotate(double angle)
{
	txt->rotate(angle);
}

vec2 animation::frame::get_size()
{
	return txt->get_size();
}

vec2 animation::frame::get_pos()
{
	return txt->get_pos();
}

float animation::frame::get_ratio()
{
	return txt->get_ratio();
}

void animation::frame::draw()
{
	txt->draw();
}

bool animation::frame::check_collision(game_object* object)
{
	return txt->check_collision(object);
}

void animation::frame::clear_collision_buffer()
{
	collided_objects.clear();
	txt->clear_collision_buffer();
}


std::shared_ptr<texture> animation::frame::get()
{
	return txt;
}

int animation::frame::get_time()
{
	return msec;
}

animation::cycle::cycle()
{
	print::loading("Creating cycle");
	print::increase_level();

	//for future 22.03.2025

	print::decrease_level();
	print::loaded("Created cycle");
}

animation::cycle::~cycle()
{
	print::info("Deleting cycle");
}

void animation::cycle::add_frame(std::shared_ptr<animation::frame> frame)
{
	if (frames.size() == 0) {
		size = frame->get_size();
	}
	frames.push_back(frame);
}

SDL_AppResult animation::cycle::update(float delta_time)
{
	for (auto& frame : frames) {
		auto app_res = frame->update(delta_time);
		switch (app_res)
		{
		case SDL_APP_CONTINUE:
			break;
		case SDL_APP_SUCCESS: return SDL_APP_SUCCESS;
			break;
		case SDL_APP_FAILURE: return SDL_APP_FAILURE;
			break;
		default:
			break;
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult animation::cycle::input(const SDL_Event* event)
{
	for (auto& frame : frames) {
		auto app_res = frame->input(event);
		switch (app_res)
		{
		case SDL_APP_CONTINUE:
			break;
		case SDL_APP_SUCCESS: return SDL_APP_SUCCESS;
			break;
		case SDL_APP_FAILURE: return SDL_APP_FAILURE;
			break;
		default:
			break;
		}
	}
	return SDL_APP_CONTINUE;
}

void animation::cycle::set_pos(vec2 pos)
{
	this->pos = pos;
	for (auto& frame : frames) {
		frame->set_pos(pos);
	}
}

void animation::cycle::move_on(vec2 velocity)
{
	this->pos = get_pos() + velocity;
	for (auto& frame : frames) {
		frame->move_on(velocity);
	}
}

void animation::cycle::set_size(vec2 size)
{
	this->size = size;
	for (auto& frame : frames) {
		frame->set_size(size);
	}
}

void animation::cycle::rotate(double angle)
{
	this->angle = angle;
	for (auto& frame : frames) {
		frame->rotate(angle);
	}
}

vec2 animation::cycle::get_size()
{
	return size;
}

vec2 animation::cycle::get_pos()
{
	return pos;
}

float animation::cycle::get_ratio()
{
	return size.x / size.y;
}

std::shared_ptr<animation::frame> animation::cycle::get()
{
	return frames[current_frame];
}

void animation::cycle::draw()
{
	get()->draw();
}

bool animation::cycle::check_collision(game_object* object)
{
	return false;
}

void animation::cycle::clear_collision_buffer()
{
	collided_objects.clear();

	for (auto& frame_ : frames) {
		frame_->clear_collision_buffer();
	}
}


void animation::cycle::play(float fps)
{
	auto frame_time = get()->get_time();
	if (!(frame_time == NULL)) {
		frames_passed++;
		auto next_frame_time = frame_time / 1000.0f * fps;

		if (frames_passed > next_frame_time) {
			if (current_frame < frames.size() - 1) {
				current_frame++;
			}
			else {
				current_frame = 0;
			}

			frames_passed = 0;
		}
	}
}

animation::player::player(SDL_Renderer* render, std::string animation_config)
{
	print::loading("Loading animation player. Config: " + animation_config);
	print::increase_level();

	using json = nlohmann::json;

	std::ifstream file(path(animation_config));
	auto animations_json = json::parse(file);
	file.close();

	for (auto& animation_type : animations_json.items()) {
		current_sprite_animation = animation_type.key();
		this->animations[current_sprite_animation] = std::make_shared<animation::cycle>();
		for (auto& animation : animations_json.at(current_sprite_animation)) {
			int time = 0;

			auto& raw_animation_time = animation["time"];

			if (raw_animation_time.is_null()) {
				time = NULL;
			}
			else {
				time = raw_animation_time;
			}

			this->animations[current_sprite_animation]->add_frame(
				std::make_shared<animation::frame>(render, animation["frame"], time));
		}
	}

	size = get()->get_size();

	print::decrease_level();
	print::loaded("Animation player loaded");
}

animation::player::~player()
{
	print::info("Deleting animation player");
}

SDL_AppResult animation::player::update(float delta_time)
{
	for (auto& [key, animation] : animations) {
		auto app_res = animation->update(delta_time);
		switch (app_res)
		{
		case SDL_APP_CONTINUE:
			break;
		case SDL_APP_SUCCESS: return SDL_APP_SUCCESS;
			break;
		case SDL_APP_FAILURE: return SDL_APP_FAILURE;
			break;
		default:
			break;
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult animation::player::input(const SDL_Event* event)
{
	for (auto& [key, animation] : animations) {
		auto app_res = animation->input (event);
		switch (app_res)
		{
		case SDL_APP_CONTINUE:
			break;
		case SDL_APP_SUCCESS: return SDL_APP_SUCCESS;
			break;
		case SDL_APP_FAILURE: return SDL_APP_FAILURE;
			break;
		default:
			break;
		}
	}
	return SDL_APP_CONTINUE;
}

void animation::player::set_pos(vec2 pos)
{
	this->pos = pos;
	for (auto& [key, animation] : animations) {
		animation->set_pos(pos);
	}
}

void animation::player::move_on(vec2 velocity)
{
	this->pos = get_pos() + velocity;
	for (auto& [key, animation] : animations) {
		animation->move_on(velocity);
	}
}

void animation::player::set_size(vec2 size)
{
	this->size = size;
	for (auto& [key, animation] : animations) {
		animation->set_size(size);
	}
}

void animation::player::rotate(double angle)
{
	this->angle = angle;
	for (auto& [key, animation] : animations) {
		animation->rotate(angle);
	}
}

vec2 animation::player::get_size()
{
	return size;
}

vec2 animation::player::get_pos()
{
	return pos;
}

float animation::player::get_ratio()
{
	return size.x / size.y;
}

std::shared_ptr<animation::cycle> animation::player::get()
{
	return animations[current_sprite_animation];
}

void animation::player::draw()
{
	get()->draw();
}

bool animation::player::check_collision(game_object* object)
{
	return false;
}

void animation::player::clear_collision_buffer()
{
	collided_objects.clear();

	for (auto& [name, animation] : animations) {
		animation->clear_collision_buffer();
	}
}

void animation::player::play(std::string name, float fps)
{
	current_sprite_animation = name;
	animations[name]->play(fps);
}


#ifndef INDEV_ANIMATIONS_DISABLED
animation::linear::linear(vec2 delta_pos, float angle, float time_ns)
	:animation::animation_type_base::animation_type_base(delta_pos, angle, time_ns)
{
	delta_time = delta_pos.len() / time_ns;
	this->delta_pos = delta_pos / delta_time;
	delta_angle = angle / delta_time;

	res_pos = delta_pos;
	res_angle = angle;

	do_event(events::on_animation_start);
}

animation::transformation animation::linear::get(float delta_time, transformation obj_tr)
{
	current_time += delta_time;
	obj_tr.pos += delta_pos;
	obj_tr.angle += delta_angle;

	if (current_time > len_ms) {
		obj_tr.pos = res_pos;
		obj_tr.angle = res_angle;
		do_event(events::on_animation_end);
	}

	return obj_tr;
}

void animation::physic_animation_player::delete_animation(size_t id)
{
	delete animations[id];
	animations[id] = nullptr;
}

bool animation::physic_animation_player::check_if_buffer_full_of_nulls()
{
	if (animations.size() == 0) {
		return true;
	}
	for (auto& an : animations) {
		if (an) {
			return false;
		}
	}

	return true;
}

void animation::physic_animation_player::auto_clear()
{
	if (check_if_buffer_full_of_nulls()) {
		animations.clear();
		return;
	}
}

animation::physic_animation_player::~physic_animation_player()
{
	for (size_t i = 0; i < animations.size(); i++) {
		delete_animation(i);
	}
}

animation::transformation animation::physic_animation_player::update(float delta_time, transformation obj_tr)
{
	transformation res;
	for (auto& anim : animations) {
		if (anim) {
			auto tr = anim->get(delta_time, obj_tr);
			res.pos += tr.pos;
			res.angle += tr.angle;
		}
	}

	auto_clear();

	return res;
}

animation::animation_type_base* animation::physic_animation_player::get(size_t id)
{
	return animations[id];
}

animation::physic_animation_player& animation::physic_animation_player::add(animation_type_base* anim_ptr)
{
	animations.emplace_back(anim_ptr);
	return *this;
}
#endif