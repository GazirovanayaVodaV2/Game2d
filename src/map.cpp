#include "map.h"

#include <memory>
#include <string>
#include <fstream>
#include <format>

#include "nlohmann/json.hpp"
#include "utils.h"
#include "SDL3/SDL_render.h"
#include "SDL3_image/SDL_image.h"
#include "wall.h"
#include "texture.h"
#include "background_sprite.h"
#include "dummy.h"
#include "projectile.h"
#include "medkit.h"
#include "basic_gun.h"
#include "ammo.h"

#define FIND_FIELD(json, type, name) if (json.find(#name) == json.end()) {print::warning("Error at line", __LINE__); print::error("Failed to find JSON field", #name);} auto name = json.at(#name).get<type>()

NLOHMANN_JSON_SERIALIZE_ENUM(map::weather_t, {
			{map::weather_t::error, nullptr},
			{map::weather_t::clear, "clear"},
			{map::weather_t::rain, "rain"},
			{map::weather_t::snow, "snow"},
			{map::weather_t::snow_storm, "snow_storm"},
	});

NLOHMANN_JSON_SERIALIZE_ENUM(basic_gun::SHOOT_MODE, {
			{basic_gun::SHOOT_MODE::ERROR, nullptr},
			{basic_gun::SHOOT_MODE::SEMI_AUTO, "semi-auto"},
			{basic_gun::SHOOT_MODE::AUTO, "auto"},
	});

map::map(atlas* atl)
{
	this->atl = atl;
	NULL_OBJECT_PTR = new NULL_OBJECT();

	sky = std::make_unique<texture>(camera::get(), SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET,
		1000, 1000);
	sky_texture = std::make_unique<texture>(camera::get(), "textures\\sky.png");
	sky_texture->set_blend(SDL_BLENDMODE_ADD);

	stars_texture = std::make_unique<texture>(camera::get(), "textures\\stars.png");
	stars_texture->set_blend(SDL_BLENDMODE_ADD);
}

map::~map()
{
	unload();

	

	delete NULL_OBJECT_PTR;
}

void map::add(game_object* obj)
{
	new_obj_buffer.push_back(std::unique_ptr<game_object>(obj));
}

void map::add_bullet(int dmg, vec2 pos, float speed, vec2 vel)
{
	auto bullet = new projectile(atl->get("bullet"), vel * speed, dmg);
	bullet->set_pos(pos);
	add(bullet);
}

game_object* map::get(vec2 pos)
{
	if (!loaded) {
		print::error("Level is not loaded!", this->name);
	}

	auto chunk_ = get_chunk(pos);
	auto& chunk_obj = chunk_->objects;
	for (auto& obj : chunk_obj) {
		if (obj->in(pos)) {
			return obj.get();
		}
	}

	return NULL_OBJECT_PTR;
}

game_object* map::get(size_t chunk_id, size_t id)
{
	if (!loaded) {
		print::error("Level is not loaded!", this->name);
	}
	return chunks.at(chunk_id)->objects.at(id).get();
}

size_t map::get_chunk_id(vec2 pos)
{
	return (size_t)convert::f2i((pos.x / tile_size / chunk_size) + (pos.y / tile_size / chunk_size) * chunks_W);;
}

map::chunk* map::get_chunk(vec2 pos)
{
	return get_chunk(get_chunk_id(pos));
}

map::chunk* map::get_chunk(size_t id)
{
	size_t ret_id = id < chunks.size() ? id : 0;
	auto& chunk_ = chunks.at(ret_id);
	if (!chunk_) {
		chunks[id] = std::make_unique<map::chunk>();
	}
	return chunk_.get();
}

map::chunk* map::get_chunk_or_null(vec2 pos)
{
	return get_chunk_or_null(get_chunk_id(pos));
}

map::chunk* map::get_chunk_or_null(size_t id)
{
	if (id >= chunks.size()) {
		return nullptr;
	}

	auto& chunk_ = chunks.at(id);
	if (!chunk_) {
		chunks[id] = std::make_unique<map::chunk>();
	}
	return chunk_.get();
}

void map::rebuild_chunks()
{
	print::info("Rebuilding chunks");
	for (size_t i = 0; i < chunks.size(); i++) {
		auto& chunk_ = chunks[i];
		
		std::erase_if(chunk_->objects, [&](auto& obj) {
			if (!obj) return true;
			auto obj_id = get_chunk_id(obj->get_pos());
			if (obj_id != i) {
				auto new_chunk = get_chunk(obj_id);
				new_chunk->objects.emplace_back(std::move(obj));

				return true;
			}
			return false;
			});
	}

	update(1.0f);
}

static rgba get_light_by_time(rgba day, rgba night, int time) {
	rgba res;
	/*
	
	0 -> 1000 - dawn
	1000 - day
	11000 -> 12000 - sunset
	12000 -> 24000 - night

	*/

	if ((time >= 0.0f) && (time <= 1000)) {
		auto dlt = time / 1000.0f;
		res = night.mix(day, dlt);
	}
	else if ((time > 1000.0f) && (time < 11000)) {
		res = day;
	}
	else if ((time >= 11000) && (time <= 12000)) {
		auto dlt = (time - 11000.0f) / 1000.0f;
		res = day.mix(night, dlt);
	}
	else {
		res = night;
	}


	return res;
}

static void draw_stars(SDL_Texture* sky, int time) {

}

static void draw_sky(map::weather_t weather_t, 
	SDL_Texture* scene, 
	const std::unique_ptr<texture>& sky, 
	const std::unique_ptr<texture>& sky_texture, 
	const std::unique_ptr<texture>& stars, 
	rgba day_b, rgba day_t, rgba night_b, rgba night_t, float time)
{

	rgba res_sky_t_color = get_light_by_time(day_t, night_t, time), 
		res_sky_b_color = get_light_by_time(day_b, night_b, time);

	auto stars_alpha = get_light_by_time( {0,0,0,0}, { 255,255,255,255 }, time).color.a;

	camera::clear(res_sky_t_color);

	camera::abjust_scale();

	sky->draw(camera::get(), NULL);
	
	sky_texture->set_color(res_sky_b_color);
	sky_texture->draw(camera::get(), NULL);

	camera::reset_scale();
}
 
void map::draw()
{
	auto scene = camera::get_scene();
	if (scene and loaded) {
		auto render = camera::get();

		camera::set_target(scene);

		draw_sky(this->weather, scene->get(), sky, sky_texture, stars_texture, {255, 255, 255}, {0, 162 , 232}, {94, 108, 127}, {39, 4, 77}, time);

		camera::abjust_scale();

		auto stars_alpha = get_light_by_time( {0,0,0,0}, { 255,255,255,255 }, time).color.a;
		stars_texture->set_alpha(stars_alpha);

		stars_texture->draw(render, NULL);

		pl->draw();
		for (auto& chunk_ : chunks) {
			for (auto& object : chunk_->objects) {
				if (auto* int_obj = dynamic_cast<interactive_object_base*>(object.get())) {
					if (!int_obj->in_inventory()) {
						int_obj->draw();
					}
				}
				else {
					object->draw();
				}
			}
		}

		if (draw_debug_info) {
			pl->draw_debug();

			for (auto& chunk_ : chunks) {

				for (auto& object : chunk_->objects) {
					object->draw_debug();
				}

			}
		}

		
		camera::reset_target();

		scene->draw(render, NULL);

		light_system->set_ambient(get_light_by_time({255,255,255}, {0,0,0}, time));
		light_system->draw();
		camera::reset_scale();
	}
}

player* map::get_player()
{
	return pl.get();
}



void map::load_level_format(std::string path_)
{
	if (!loaded) {
		print::loading("map");
		print::increase_level();

		pl = std::make_unique<player>(camera::get(), "entity\\player\\animations.json");
		light_system = std::make_unique<light::system>();

		using json = nlohmann::json;
		std::ifstream file(path(path_));
		if (!file.is_open()) {
			print::error("Map open failed, path", path_);
		}
		auto json_level = json::parse(file);
		file.close();

		json_level.at("name").get_to(name);
		json_level.at("type").get_to(level_type);

		print::warning("W, H and TILE_SIZE is deprecated");
		json_level.at("W").get_to(W);
		json_level.at("H").get_to(H);
		json_level.at("tile_size").get_to(tile_size);

		chunks_W = std::ceilf((float)W / chunk_size);
		chunks_H = std::ceilf((float)H / chunk_size);

		chunks.resize(chunks_W * chunks_H);
		height_map.resize(W);
		std::fill(height_map.begin(), height_map.end(), std::numeric_limits<float>::max());

		struct layer {
			std::string name;
			std::vector<int> ids;
		};

		struct level_switcher_data {
			std::string id, level_name;
		};

		std::vector<layer> layers;
		std::vector<level_switcher_data> level_switchers;

		auto ids = json_level.at("id").get<std::map<std::string, json>>();

		auto environment_settings = json_level.value("environment", nlohmann::json::object());
		this->weather = environment_settings.value("weather", weather_t::clear);
		this->time = (float)environment_settings.value("time", 0);
		this->time_cycle = environment_settings.value("time_cycle", false);


		if (json_level.find("level_switchers") != json_level.end()) {
			for (auto& [name, id] : json_level.at("level_switchers").items()) {
				level_switchers.emplace_back(level_switcher_data{
					.id = id,
					.level_name = name
					});
			}
		}

		if (json_level.find("layers") != json_level.end()) {
			for (auto& [name, data] : json_level.at("layers").items()) {
				if (name == "level_switchers") continue;
				layers.emplace_back(layer{
					.name = name,
					.ids = data.get<std::vector<int>>()
					});
			}

			for (auto& layer : layers) {
				int index = 0;
				for (auto& id : layer.ids) {
					auto& block = ids[std::to_string(id)];

					auto& type = block.at("type");

					auto pos = vec2((index % W) * tile_size, (index / W) * tile_size);
					auto size = vec2(tile_size, tile_size);

					index++;

					game_object* object = nullptr;

					if (type == keyword_to_string(air)) {
						continue;
					}
					else if (type == keyword_to_string(wall)) {
						auto sprite_id = block.at("sprite_id").get<std::string>();

						object = new wall(atl->get(sprite_id));
						object->set_pos(pos);
						object->set_size(size);

						int x_i = convert::f2i(pos.x / size.x);
						if (pos.y < height_map[x_i]) {
							height_map[x_i] = pos.y;
						}
					}
					else if (type == keyword_to_string(player)) {
						size = vec2(tile_size, tile_size * 2);
						pl->set_pos(pos);
						pl->set_size(size);

						continue;
					}
					else if (type == keyword_to_string(light::source)) {
						auto radius = block.at("radius").get<float>();
						hex color = 0xffffffff;

						if (block.find("color") != block.end()) {
							auto str = block.at("color").dump();
							str = str.substr(2, 8);
							color = (hex)std::stoul(str, nullptr, 16);
						}

						light_system->add_light("textures\\lightsource.png", radius, color);
						light_system->get_last()->set_pos(pos - vec2((tile_size / 2), (tile_size / 2)));

						continue;
					}
					else if (type == keyword_to_string(background_sprite)) {
						auto sprite_id = block.at("sprite_id").get<std::string>();

						object = new background_sprite(atl->get(sprite_id));
						
						object->set_pos(pos);
						object->set_size(size);
					}
					else if (type == keyword_to_string(dummy)) {
						object = new dummy(atl->get(keyword_to_string(dummy)), 100);
						object->set_pos(pos);
					}
					else if (type == keyword_to_string(medkit)) {
						object = new medkit(atl->get(keyword_to_string(medkit)));
						object->set_pos(pos);
					}

					else if (type == keyword_to_string(basic_gun)) {
						FIND_FIELD(block, int, damage);
						FIND_FIELD(block, basic_gun::SHOOT_MODE, mode);
						FIND_FIELD(block, int, max_mag);

						object = new basic_gun(atl->get(keyword_to_string(basic_gun)), damage, mode, max_mag);
						object->set_pos(pos);
					}
					else if (type == keyword_to_string(ammo)) {
						FIND_FIELD(block, int, ammo_type);
						FIND_FIELD(block, int, count);

						object = new ammo(atl->get(keyword_to_string(ammo)), ammo_type, count);

						object->set_pos(pos);
					}
					add(object);
				}
			}
		}
		

		print::decrease_level();
		print::loaded("Map loaded");

		loaded = true;
	}
}



void map::load(std::string path_)
{
	after_load_delay = 0;
	if (path_.find(".level") != std::string::npos) {
		load_level_format(path_);
	}

	for (size_t i = 0; i < chunks.size(); i++) {
		if (!chunks[i])
			chunks[i] = std::make_unique<map::chunk>();
	}
}

void map::unload()
{
	if (loaded) {
		print::info("Deleting map");

		chunks.clear();
		chunks.shrink_to_fit();

		height_map.clear();
		height_map.shrink_to_fit();

		light_system.reset();
		pl.reset();
	}

	loaded = false;
}

SDL_AppResult map::update(float delta_time)
{
	if (loaded) {
		for (auto& chunk_ : chunks) {
			for (auto& obj1 : chunk_->objects) {
				auto current_chunk = get_chunk_or_null(obj1->get_pos());

				pl->check_collision(obj1.get());
				obj1->update(delta_time);

				if (current_chunk) {
					for (auto& obj2 : current_chunk->objects) {
						if (obj1.get() == obj2.get()) continue;
						(obj1)->check_collision(obj2.get());
					}
				}
			}
		}

		for (auto& chunk_ : chunks) {
			auto& objects = chunk_->objects;
			objects.erase(
				std::remove_if(objects.begin(), objects.end(),
					[](const std::unique_ptr<game_object>& obj) {
						return !obj->exist;
					}),
				objects.end());
		}

		for (auto& new_obj : new_obj_buffer) {
			auto chunk_ = get_chunk(new_obj->get_pos());

			chunk_->objects.emplace_back(std::move(new_obj));
		
		}
		new_obj_buffer.clear();

		//To prevent player teleport to void after spawn
		if (after_load_delay < 10) {
			after_load_delay++;
		}
		else {
			pl->update(delta_time);
		}

		if (time_cycle) {
			//time += fps::synch<float>(0.55f); //1 day tooks 12 minutes
			time += fps::synch<float>(10.0f);
			if (time > 24000.0f) {
				time = 0.0f;
			}
		}
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult map::input(const SDL_Event* event)
{
	if (loaded) {
		for (auto& chunk_ : chunks)
		{
			for (auto& obj : chunk_->objects) {
				obj->input(event);
			}
		}
		
		if (event->type == SDL_EVENT_KEY_DOWN) {
			switch (event->key.key)
			{
			case SDLK_F5: {
				draw_debug_info = !draw_debug_info;
			} break;
			case SDLK_T: {
				rebuild_chunks();
			} break;
			default: {
			} break;
			}
		}

		return pl->input(event);
	}
	return SDL_APP_CONTINUE;
}

//Level manager

std::map<std::string, std::unique_ptr<map>> level_manager::levels;
std::map<std::string, std::string> level_manager::paths;

std::string level_manager::current_level;

atlas* level_manager::atl = nullptr;

std::string level_manager::get_level_name_from_file(std::string path_)
{

	using json = nlohmann::json;
	std::ifstream file(path(path_));
	if (!file.is_open()) {
		print::error("Failed to open map for name reading", path_);
	}
	auto json_level = json::parse(file);
	file.close();

	return json_level.at("name");
}

level_manager::~level_manager()
{
	unload_all();
}

void level_manager::set_atlas(atlas* atl_)
{
	atl = atl_;
}

void level_manager::add(std::string path_)
{

	auto name = get_level_name_from_file(path_);
	levels[name] = std::make_unique<map>(atl);

	paths[name] = path_;
}

void level_manager::load(std::string name)
{
	levels.at(name)->load(paths.at(name));
	current_level = name;
}

void level_manager::unload_all()
{
	for (auto& [name, level] : levels) {
		level->unload();
	}

	current_level.clear();
}

void level_manager::draw()
{
	if (!current_level.empty()) {
		get()->draw();
	}
}

SDL_AppResult level_manager::update(float delta_time)
{
	if (!current_level.empty()) {
		return get()->update(delta_time);
	}
	return SDL_APP_SUCCESS;
}

SDL_AppResult level_manager::input(const SDL_Event* event)
{
	if (!current_level.empty()) {
		return get()->input(event);
	}

	return SDL_APP_SUCCESS;
}

map* level_manager::get()
{
	if (current_level.empty()) {
		print::error("Current level is NULL");
	}

	return levels.at(current_level).get();
}

bool level_manager::is_any_level_loaded()
{
	return current_level.size() > 0;
}

bool level_manager::is_level_empty()
{
	return current_level.empty();
}
