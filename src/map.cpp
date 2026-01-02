#include "map.h"

#include <utils.h>
#include "SDL3/SDL_render.h"

#include <nlohmann/json.hpp>
#include <fstream>

#include <wall.h>
#include <memory>
#include <string>
#include "texture.h"
#include "background_sprite.h"

#include "dummy.h"

map::~map()
{
	unload();
}

void map::add(std::shared_ptr<game_object> obj)
{
	new_obj_buffer.push_back(obj);
}

std::shared_ptr<game_object>& map::get(vec2 pos)
{
	if (!loaded) {
		throw std::format("Level is not loaded! Name: {}", this->name).c_str();
	}

	size_t i = 0;
	if (objects.size() > 0)
	{
		for (; i < objects.size() - 1; i++) {
			auto& obj = objects[i];
			if (obj->in(pos)) {
				return objects[i];
			}
		}
	}

	return NULL_OBJECT_PTR;
}

std::shared_ptr<game_object>& map::get(size_t id)
{
	if (!loaded) {
		throw std::format("Level is not loaded! Name: {}", this->name).c_str();
	}
	if (id > objects.size()) {
		throw std::format("Out of objects! Index: {}", id).c_str();
	}
	return objects[id];
}

void map::draw()
{
	if (scene and loaded) {
		auto viewport = camera::get_viewport();
		auto render = camera::get();
		auto res_viewport = viewport;
		res_viewport.x = 0;
		res_viewport.y = 0;

		SDL_SetRenderTarget(render, scene);
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderClear(render);

		SDL_SetRenderDrawColor(render, 255, 255, 255, 255);

		SDL_SetRenderScale(render, 1000.0f / res_viewport.w, 1000.0f / res_viewport.h);

		pl->draw();
		for (auto& object : objects) {
			if (object->exist) {
				object->draw();
			}
		}

		SDL_SetRenderScale(render, 1.0f, 1.0f);
		SDL_SetRenderTarget(render, NULL);

		SDL_RenderTexture(render, scene, NULL, &res_viewport);

		light_system->draw();


		if (draw_debug_info) {
			pl->draw_debug();
			for (auto& object : objects) {
				if (object->exist) {
					object->draw_debug();
				}
			}
		}
	}
}

std::shared_ptr<player>& map::get_player()
{
	return pl;
}



void map::load_level_format(std::string path_, std::shared_ptr<atlas>& txt_context)
{
	if (!loaded) {
		print::loading("map");
		print::increase_level();

		scene = SDL_CreateTexture(camera::get(), SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, 1000, 1000);

		SDL_SetTextureScaleMode(scene, SDL_SCALEMODE_NEAREST);

		pl = std::make_shared<player>(camera::get(), "entity\\player\\animations.json");
		light_system = std::make_shared<light::system>();

		using json = nlohmann::json;
		std::ifstream file(path(path_));
		if (!file.is_open()) {
			throw std::format("Map open failed, path: {}", path_).c_str();
		}
		auto json_level = json::parse(file);
		file.close();

		json_level.at("name").get_to(name);
		json_level.at("type").get_to(level_type);

		print::warning("W, H and TILE_SIZE is deprecated");
		json_level.at("W").get_to(W);
		json_level.at("H").get_to(H);
		json_level.at("tile_size").get_to(tile_size);

		struct id {
			json data;
		};

		struct layer {
			std::string name;
			std::vector<int> ids;
		};

		struct level_switcher_data {
			std::string id, level_name;
		};

		std::map<int, id> ids;
		std::vector<layer> layers;
		std::vector<level_switcher_data> level_switchers;

		auto& raw_ids = json_level.at("id");
		auto raw_ids_items = raw_ids.items();

		for (auto& raw_id : raw_ids_items) {
			auto& str_id = raw_id.key();
			id id_{raw_ids.at(str_id) };
			ids[std::stoi(str_id)] = id_;
		}

		if (json_level.find("level_switchers") != json_level.end()) {
			auto& raw_ls_data = json_level.at("level_switchers");
			auto raw_ls_items = raw_ls_data.items();

			for (auto& raw_ls : raw_ls_items) {
				auto ls_id = raw_ls.key();

				level_switcher_data ls_data = {
					.id = ls_id,
					.level_name = raw_ls_data.at(ls_id)
				};

				level_switchers.push_back(ls_data);
			}
		}

		if (json_level.find("layers") != json_level.end()) {
			//print::warning("Layers in maps is deprecated!");

			auto& raw_layers = json_level.at("layers");
			auto raw_layers_items = raw_layers.items();

			for (auto& raw_layer : raw_layers_items) {
				auto& layer_name = raw_layer.key();

				if (layer_name == "level_switchers") {

				} else {
					layer layer_;
					layer_.name = layer_name;
					raw_layers.at(layer_name).get_to(layer_.ids);

					layers.push_back(layer_);
				}

			}

			for (auto& layer : layers) {
				int index = 0;
				for (auto& id : layer.ids) {
					auto& block = ids[id];

					auto& type = block.data.at("type");

					using namespace OBJECT::MAP;
					if (type == keyword_to_string(air)) {

					}
					else if (type == "brick") {
						std::string sprite_id;
						block.data.at("sprite_id").get_to(sprite_id);

						auto object = std::make_shared<wall>(txt_context->get(sprite_id));
						//size_t i = (ipos.x * W) + ipos.x;
						auto pos = vec2((index % W) * tile_size, (index / W) * tile_size);
						auto size = vec2(tile_size, tile_size);
						object->set_pos(pos);
						object->set_size(size);

						add(object);
					}
					else if (type == keyword_to_string(player_spawn)) {
						auto pos = vec2((index % W) + (tile_size), (index / W) + (tile_size));
						auto size = vec2(tile_size, tile_size * 2);
						pl->set_pos(pos);
						pl->set_size(size);
					}
					else if (type == keyword_to_string(light)) {

						float radius;
						block.data.at("radius").get_to(radius);
						light_system->add_light("textures\\lightsource.png", radius, 0xffffffff);

						auto pos = vec2(((index % W) * tile_size) - (tile_size / 2), ((index / W) * tile_size) - (tile_size / 2));
						//pos = pos * radius;
						light_system->get_last()->set_pos(pos);
					}
					else if (type == keyword_to_string(background_sprite)) {
						std::string sprite_id;
						block.data.at("sprite_id").get_to(sprite_id);
						
						auto object = std::make_shared<background_sprite>(txt_context->get(sprite_id));
						//size_t i = (ipos.x * W) + ipos.x;
						auto pos = vec2((index % W) * tile_size, (index / W) * tile_size);
						auto size = vec2(tile_size, tile_size);
						object->set_pos(pos);
						object->set_size(size);

						add(object);
					}
					else if (type == keyword_to_string(dummy_entity)) {
						auto object = std::make_shared<dummy>(txt_context, 100);
						auto pos = vec2((index % W) * tile_size, (index / W) * tile_size);
						object->set_pos(pos);
						add(object);
					}

					index++;
				}
			}
		}
		

		print::decrease_level();
		print::loaded("Map loaded");

		loaded = true;
	}
}

void map::load(std::string path_, std::shared_ptr<atlas>& txt_context)
{
	atl = txt_context;
	if (path_.find(".level") != std::string::npos) {
		load_level_format(path_, txt_context);
	}
}

void map::unload()
{
	if (loaded) {
		print::info("Deleting map");
		SDL_DestroyTexture(scene);

		objects.clear();
		light_system.reset();
		pl.reset();
	}

	loaded = false;
}

SDL_AppResult map::update(float delta_time)
{
	if (loaded) {
		for (auto& object : objects) {
			//pl->check_collision(object);
			if (object->exist) {
				pl->check_collision(object.get());
				auto app_res = object->update(delta_time);
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
		}

		
		for (auto& obj1 : objects) {
			if (!obj1->exist) continue;
			for (auto& obj2 : objects) {
				if (obj1 == obj2 || !obj2->exist) continue;
				obj1->check_collision(obj2.get());
			}
		}

		objects.erase(
			std::remove_if(objects.begin(), objects.end(),
				[](const std::shared_ptr<game_object>& obj) {
					return !obj->exist;
				}),
			objects.end());

		if (!new_obj_buffer.empty()) {
			objects.insert(objects.end(), new_obj_buffer.begin(), new_obj_buffer.end());
			new_obj_buffer.clear();
		}

		return pl->update(delta_time);
	}
	


	return SDL_APP_CONTINUE;
}

SDL_AppResult map::input(const SDL_Event* event)
{
	if (loaded) {
		for (auto& obj : objects) {
			if (obj->exist) {
				obj->input(event);
			}
		}
		
		if (event->type == SDL_EVENT_KEY_DOWN) {
			switch (event->key.key)
			{
			case SDLK_F5: {
				draw_debug_info = !draw_debug_info;
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

std::map<std::string, std::shared_ptr<map>> level_manager::levels;
std::map<std::string, std::shared_ptr<atlas>> level_manager::textures;
std::map<std::string, std::string> level_manager::paths;

std::string level_manager::current_level;

std::string level_manager::get_level_name_from_file(std::string path_)
{

	using json = nlohmann::json;
	std::ifstream file(path(path_));
	if (!file.is_open()) {
		throw std::format("Failed to open map for name reading, path: {}", path_).c_str();
	}
	auto json_level = json::parse(file);
	file.close();

	return json_level.at("name");
}

level_manager::~level_manager()
{
	unload_all();
}

void level_manager::add(std::string path_, std::shared_ptr<atlas>& atl)
{

	auto name = get_level_name_from_file(path_);
	levels[name] = std::make_shared<map>();

	//For easy reloading
	textures[name] = atl;
	paths[name] = path_;
}

void level_manager::load(std::string name)
{
	levels.at(name)->load(paths.at(name), textures.at(name));
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

std::shared_ptr<map>& level_manager::get()
{
	if (current_level.empty()) {
		throw std::format("Current level is NULL").c_str();
	}

	return levels.at(current_level);
}

bool level_manager::is_level_empty()
{
	return current_level.empty();
}
