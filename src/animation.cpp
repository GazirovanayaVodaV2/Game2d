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
	txt = std::make_shared<texture>(render, path);
	this->path = path;

	print::decrease_level();
	print::loaded();
}

animation::frame::~frame()
{
	print::info("Deleting frame. Path: " + path);
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
	frames.push_back(frame);
}


std::shared_ptr<animation::frame> animation::cycle::get()
{
	return frames[current_frame];
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

	print::decrease_level();
	print::loaded("Animation player loaded");
}

animation::player::~player()
{
	print::info("Deleting animation player");
}

std::shared_ptr<animation::cycle> animation::player::get()
{
	return animations[current_sprite_animation];
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