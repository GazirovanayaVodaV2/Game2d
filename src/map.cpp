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
#include "projectile.h"
#include "medkit.h"

map::~map()
{
	unload();
}

void map::add(std::shared_ptr<game_object> obj)
{
	new_obj_buffer.push_back(obj);
}

void map::add_bullet(int dmg, vec2 pos, float speed, vec2 vel)
{
	auto bullet = std::make_shared<projectile>(get_atlas()->get("bullet"), vel * speed, dmg);
	bullet->set_pos(pos);
	add(bullet);
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
				if (object->get_type() == OBJECT::TYPE::INTERACTIVE_OBJECT 
					or is_subtype_of(object->get_type(), OBJECT::TYPE::INTERACTIVE_OBJECT)) {
					if (not std::static_pointer_cast<interactive_object_base>(object)->in_inventory()) {
						object->draw();
					}
				}
				else {
					object->draw();
				}
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

		struct layer {
			std::string name;
			std::vector<int> ids;
		};

		struct level_switcher_data {
			std::string id, level_name;
		};

		std::vector<layer> layers;
		std::vector<level_switcher_data> level_switchers;

		//std::map<int, json> ids;
		//for (auto& [str_id, data] : json_level.at("id").items()) {
		//	ids[std::stoi(str_id)] = data;
		//}

		auto ids = json_level.at("id").get<std::map<std::string, json>>();

		if (json_level.find("level_switchers") != json_level.end()) {
			for (auto& [name, id] : json_level.at("level_switchers").items()) {
				level_switchers.emplace_back(level_switcher_data{
					.id = id,
					.level_name = name
					});
			}
		}

		if (json_level.find("layers") != json_level.end()) {
			//print::warning("Layers in maps is deprecated!");


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

					std::shared_ptr<game_object> object;

					if (type == keyword_to_string(air)) {
						continue;
					}
					else if (type == keyword_to_string(wall)) {
						auto sprite_id = block.at("sprite_id").get<std::string>();

						object = std::make_shared<wall>(txt_context->get(sprite_id));
						object->set_pos(pos);
						object->set_size(size);
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

						object = std::make_shared<background_sprite>(txt_context->get(sprite_id));
						
						object->set_pos(pos);
						object->set_size(size);
					}
					else if (type == keyword_to_string(dummy)) {
						object = std::make_shared<dummy>(txt_context, 100);
						object->set_pos(pos);
					}
					else if (type == keyword_to_string(medkit)) {
						object = std::make_shared<medkit>(txt_context);
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

void map::load(std::string path_, std::shared_ptr<atlas>& txt_context)
{
	after_load_delay = 0;
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

		//To prevent player teleport to void after spawn
		if (after_load_delay < 10) {
			after_load_delay++;
		}
		else {
			pl->update(delta_time);
		}
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

bool level_manager::is_any_level_loaded()
{
	return current_level.size() > 0;
}

bool level_manager::is_level_empty()
{
	return current_level.empty();
}
