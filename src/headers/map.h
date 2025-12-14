#pragma once
#include "base.h"
#include <game_object.h>
#include <window.h>
#include <texture.h>
#include <entity.h>
#include <light_system.h>

#include <vector>
#include <list>
#include <string>
#include <memory>

class map :
    public base
{
private:
    std::vector<std::shared_ptr<game_object>> objects;

    SDL_Texture* scene = nullptr;
    std::shared_ptr<player> pl;
    std::shared_ptr<light::system> light_system;

    std::shared_ptr<game_object> NULL_OBJECT_PTR;

    std::string name;
    std::string level_type = "normal";
    int W = 0, H = 0, tile_size = 64;

    bool loaded = false;

    void load_level_format(std::string path_, std::shared_ptr<atlas>& txt_context);
protected:
    static inline const OBJECT::TYPE type = OBJECT::TYPE::MAP;
public:
    map() {
        NULL_OBJECT_PTR = std::make_shared<NULL_OBJECT>();
    };
	~map() override;

    std::shared_ptr<game_object>& get(vec2 pos);
    std::shared_ptr<game_object>& get(size_t id);
	void draw();
    void add(std::shared_ptr<game_object> obj);

    std::shared_ptr<player>& get_player();

    void load(std::string path_, std::shared_ptr<atlas>& txt_context);
    void unload();
    
    SDL_AppResult update(float delta_time);
    SDL_AppResult input(const SDL_Event* event);

    OBJECT::TYPE get_type() override;
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

    static std::map<std::string, std::shared_ptr<map>> levels;

    //For easy reloading
    static std::map<std::string, std::shared_ptr<atlas>> textures;
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

    static void add(std::string path_, std::shared_ptr<atlas>& atl);

    static void load(std::string name);
    static void unload_all();

    static void draw();
    static SDL_AppResult update(float delta_time);
    static SDL_AppResult input(const SDL_Event* event);

    static std::shared_ptr<map>& get();

    static bool is_level_empty();
};

