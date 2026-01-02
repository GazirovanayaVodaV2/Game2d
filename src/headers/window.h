#pragma once
#include "base_object.h"
#include <game_object.h>

#include "with_default_value.h"

#include <rgba.h>
#include <string>

class window {
private:
    static SDL_Window* sdl_window;

    static with_default_value<vec2> size;
    static int fps;
    static bool fullscreen;

    window();
    ~window();
public:
    static window& get_instance() {
        static window inst;
        return inst;
    }
    
    static SDL_AppResult update(float delta_time);
    static SDL_AppResult input(const SDL_Event* event);
    static void set_pos(vec2 pos);
    static void move_on(vec2 velocity);
    static void set_size(vec2 size);
    static void rotate(double angle) {};

    static vec2 get_pos();
    static vec2 get_size();
    static float get_ratio();

    static SDL_Window* get();

    static OBJECT::TYPE get_type();

    static int get_fps();
};

class camera {
private:
    static SDL_Renderer* sdl_renderer;
    static vec2 pos;
    static with_default_value<vec2> size;
    static std::shared_ptr<game_object> connected_object;
    static SDL_FRect viewport;
    static bool show_gui;

    camera();
    ~camera();
public:
    static camera& get_instance() {
        static camera inst;
        return inst;
    }

	static SDL_AppResult update(float delta_time);
    static SDL_AppResult input(const SDL_Event* event);
	
    static void set_pos(vec2 pos_);
    static void move_on(vec2 velocity_);

    static void set_size(vec2 size_);
    static void set_scale(vec2 size_);

    static void rotate(double angle_);

    static vec2 get_size();
    static vec2 get_pos();
    static vec2 get_scale();

    static float get_ratio();
    static OBJECT::TYPE get_type();

    static SDL_Renderer* get();

    static void connect_object(std::shared_ptr<game_object> object);

    static void clear();
    static void clear(unsigned int color);
    static void clear(rgba color);
    static void set_color(unsigned int color);
    static void set_color(rgba color);
    static void present();

    static void set_viewport(SDL_FRect viewport);
    static void reset_viewport();
    static void restore_viewport();
    static SDL_FRect get_viewport();
    static SDL_FRect* get_viewport_ptr();

    static vec2 get_mouse_relative_pos(float m_x, float m_y);

    static void draw_debug_text(std::string text, vec2 pos);
};