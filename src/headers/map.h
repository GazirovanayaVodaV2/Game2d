#pragma once
#include "base.h"
#include <game_object.h>
#include <window.h>
#include <texture.h>
#include <entity.h>
#include <player.h>
#include <light_system.h>

#include <vector>
#include <list>
#include <string>
#include <memory>

class map :
    public base
{
public:
    enum class weather_t {
        clear,
        rain,
        snow,
        snow_storm,
        error
    };
private:
    std::vector<std::shared_ptr<game_object>> objects;
    std::vector<std::shared_ptr<game_object>> new_obj_buffer;
    std::vector<float> height_map;

    atlas* atl;

    SDL_Texture* scene = nullptr;
    std::unique_ptr<player> pl;
    std::unique_ptr<light::system> light_system;

    std::shared_ptr<game_object> NULL_OBJECT_PTR;

    std::string name;
    std::string level_type = "normal";
    int W = 0, H = 0, tile_size = 64;

    float time = 0;
    bool time_cycle = true;
    weather_t weather = weather_t::clear;

    std::unique_ptr<texture> sky;
    std::unique_ptr<texture> sky_texture;
    std::unique_ptr<texture>stars_texture;

    bool loaded = false;
    bool draw_debug_info = DEBUG_VAL(true, false);

    int after_load_delay = 0;

    void load_level_format(std::string path_);
protected:
    const OBJECT::TYPE type = OBJECT::TYPE::MAP;
public:
    map(atlas* atl);
	~map() override;

    std::shared_ptr<game_object>& get(vec2 pos);
    std::shared_ptr<game_object>& get(size_t id);
	void draw();
    void add(std::shared_ptr<game_object> obj);

    void add_bullet(int dmg, vec2 pos, float speed, vec2 vel);

    player* get_player();
    const std::vector<std::shared_ptr<game_object>>& get_objects() {
        return objects;
    }

    void load(std::string path_);
    void unload();
    
    SDL_AppResult update(float delta_time);
    SDL_AppResult input(const SDL_Event* event);

    atlas* get_atlas() { return atl; };

    OBJECT::TYPE get_type() override { return OBJECT::TYPE::MAP; }
};

/*
class object_atlas {
private:
    std::vector<game_object*> objects;
public:
    object_atlas() = default;
    ~object_atlas() {
        for (auto& obj : objects) {
            delete obj;
        }
    }

    object_atlas& add(game_object* obj) {
        objects.emplace_back(obj);
        return *this;
    }

    std::shared_ptr<game_object> get_copy(size_t id) {
        return std::make_shared<game_object>(*(objects.at(id)));
    }
};*/

class level_manager {
private:
    //object_atlas object_atlas_;

    static atlas* atl;

    static std::map<std::string, std::unique_ptr<map>> levels;

    //For easy reloading
    static std::map<std::string, std::string> paths;

    static std::string current_level;

    static std::string get_level_name_from_file(std::string path_);

    level_manager() = default;
    ~level_manager();
public:
    static level_manager& get_instance() {
        static level_manager instance;
        return instance;
    }

    static void set_atlas(atlas* atl_);

    static void add(std::string path_);

    static void load(std::string name);
    static void unload_all();

    static void draw();
    static SDL_AppResult update(float delta_time);
    static SDL_AppResult input(const SDL_Event* event);

    static map* get();

    static bool is_any_level_loaded();

    static bool is_level_empty();
};

