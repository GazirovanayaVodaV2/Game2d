#include "window.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <format>
#include <iostream>

#include <utils.h>
#include "SDL3/SDL_rect.h"

//Window

SDL_Window* window::sdl_window = nullptr;
with_default_value<vec2> window::size = vec2(1280, 720);
int window::fps = 60;
bool window::fullscreen = false;

window::window()
{
	print::loading("Creating window");
	using json = nlohmann::json;
	std::ifstream json_file(path("configs\\window.conf"));
	auto parsed_options = json::parse(json_file);
	json_file.close();

	auto window_name = parsed_options.at("window_name").get<std::string>();
	auto resolution = parsed_options.at("resolution").get<std::vector<int>>();
	auto vsync = parsed_options.at("vsync").get<bool>();
	auto resizeable = parsed_options.at("resizeable").get<bool>();
	
	parsed_options.at("fps").get_to(fps);
	parsed_options.at("fullscreen").get_to(fullscreen);
	

	if (vsync) {
		fps = -1;
	}

	auto window_flags = NULL | (SDL_WINDOW_FULLSCREEN * fullscreen) | (SDL_WINDOW_RESIZABLE * resizeable);
	sdl_window = SDL_CreateWindow(window_name.c_str(), size.get_default().x, size.get_default().y, window_flags);
	if (!sdl_window) {
		throw std::format("Window creating error: {} Code: {}", SDL_GetError(), static_cast<int>(SDL_APP_FAILURE));
	}
	
	SDL_SetWindowSize(sdl_window, resolution[0], resolution[1]);
	SDL_SetWindowMinimumSize(sdl_window, size.get_default().x, size.get_default().y);

	print::loaded("Window created!");
}

window::~window()
{
	print::info("Destroying window");
	SDL_DestroyWindow(sdl_window);
}

SDL_AppResult window::update(float delta_time)
{
	return SDL_APP_CONTINUE;
}

SDL_AppResult window::input(const SDL_Event* event)
{
	if (event->type == SDL_EVENT_WINDOW_RESIZED) {
		int w, h;
		SDL_GetWindowSizeInPixels(sdl_window, &w, &h);

		(*size).x = convert::i2f(w);
		(*size).y = convert::i2f(h);

		//camera::set_size(size);

		//SDL_SetRenderLogicalPresentation(camera::get(), w, h, SDL_LOGICAL_PRESENTATION_OVERSCAN);
	}

	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	if (event->type == SDL_EVENT_KEY_DOWN) {
		switch (event->key.key)
		{
		case SDLK_F11: {
			fullscreen = !fullscreen;
			SDL_SetWindowFullscreen(sdl_window, fullscreen);
		} break;
		default:
			break;
		}
	}

	return SDL_APP_CONTINUE;
}

void window::set_pos(vec2 pos)
{
	auto int_pos = pos.get_int();
	SDL_SetWindowPosition(sdl_window, int_pos.x, int_pos.y);
}

void window::move_on(vec2 velocity)
{
	auto summ = velocity + get_pos();
	set_pos(summ);
}

void window::set_size(vec2 size)
{
	auto int_size = size.get_int();
	SDL_SetWindowSize(sdl_window, int_size.x, int_size.y);
}

vec2 window::get_pos()
{
	int x, y;
	SDL_GetWindowPosition(sdl_window, &x, &y);
	return vec2(x, y);
}

vec2 window::get_size()
{
	int w, h;
	SDL_GetWindowSize(sdl_window, &w, &h);
	return vec2(w,h);
}

float window::get_ratio()
{
	auto size = get_size();
	return size.x / size.y;
}

SDL_Window *window::get()
{
    return sdl_window;
}

OBJECT::TYPE window::get_type()
{
	return OBJECT::TYPE::WINDOW;
}

int window::get_fps()
{
	return fps;
}

//Camera

SDL_Renderer* camera::sdl_renderer = nullptr;
vec2 camera::pos = vec2(0,0);
vec2 camera::win_res = vec2(0, 0);
game_object* camera::connected_object = nullptr;
SDL_FRect camera::viewport = { 0 };

SDL_Texture* camera::scene = nullptr;

bool camera::show_gui = DEBUG_VAL(true, false);
std::string camera::debug_text = "";

camera::camera()
{
	print::loading("Creating renderer");

	using json = nlohmann::json;
	std::ifstream json_file(path("configs\\window.conf"));
	auto parsed_options = json::parse(json_file);
	json_file.close();

	auto vsync = parsed_options.at("vsync").get<bool>();

	sdl_renderer = SDL_CreateRenderer(window::get_instance().get(), NULL);
	if (!sdl_renderer) {
		throw std::format("Renderer creating error: {} Code: {}", SDL_GetError(), static_cast<int>(SDL_APP_FAILURE));
	}

	if (!SDL_SetRenderVSync(sdl_renderer, vsync)) {
		throw std::format("Renderer vsync error: {} Code: {}", SDL_GetError(), static_cast<int>(SDL_APP_FAILURE));
	}

	//SDL_SetRenderLogicalPresentation(camera::get(), 1000, 1000, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	
	viewport = pos.get_frect(win_res);

	scene = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, 1000, 1000);
	SDL_SetTextureScaleMode(scene, SDL_SCALEMODE_NEAREST);

	print::loaded("Renderer created!");
}

camera::~camera()
{
	print::info("Destroying camera");
	SDL_DestroyTexture(scene);
	SDL_DestroyRenderer(sdl_renderer);
}

SDL_AppResult camera::update(float delta_time)
{
	win_res = window::get_instance().get_size();
	//set_size(win_resolution);
	//set_scale(win_resolution);

	if (connected_object) {
		auto obj_center = (connected_object->get_size() / 2.0f);
		auto new_pos = connected_object->get_pos() + vec2(obj_center.x / get_ratio(), obj_center.y) - vec2(500.0f * get_ratio(), 500.0f);
		new_pos = vec2(-new_pos.x, -new_pos.y);
		set_pos(new_pos);
	}

	viewport = pos.get_frect(win_res);

    return SDL_APP_SUCCESS;
}

SDL_AppResult camera::input(const SDL_Event *event)
{
	if (event->type == SDL_EVENT_KEY_DOWN) {
		auto camera_velocity = 5;
		switch (event->key.key)
		{
		case SDLK_F5: {
			show_gui = !show_gui;
			if (bench::is_enabled()) bench::disable();
			else bench::enable();
		} break;
		default:
			break;
		}

		if (!connected_object) {
			switch (event->key.key)
			{
			case SDLK_W: {
				move_on(vec2(0, camera_velocity));
			} break;
			case SDLK_S: {
				move_on(vec2(0, -camera_velocity));
			} break;
			case SDLK_D: {
				move_on(vec2(-camera_velocity, 0));
			} break;
			case SDLK_A: {
				move_on(vec2(camera_velocity, 0));
			} break;
			default:
				break;
			}
		}
	}
	else if (event->type == SDL_EVENT_KEY_UP) {
		switch (event->key.key)
		{
		default:
			break;
		}
	}
    return SDL_APP_SUCCESS;
}

void camera::set_pos(vec2 pos_)
{
	pos = pos_;
}

void camera::move_on(vec2 velocity_)
{
	set_pos(get_pos() + velocity_);
}

void camera::set_scale(vec2 size_)
{
	SDL_SetRenderScale(sdl_renderer, win_res.x / size_.x,
		win_res.y / size_.y);
}

void camera::abjust_scale()
{
	//SDL_SetRenderScale(sdl_renderer, win_res.x / size.x,
	//	win_res.y / (*size).y);

	SDL_SetRenderScale(sdl_renderer, 1.0f / get_ratio(),
		1.0f);
}

void camera::reset_scale()
{
	SDL_SetRenderScale(sdl_renderer, 1, 1);
}

void camera::rotate(double angle)
{
}

vec2 camera::get_scale()
{
	float w, h;

    SDL_GetRenderScale(sdl_renderer, &w, &h);

	return vec2(w,h);
}

vec2 camera::get_pos()
{
	return pos;
}

float camera::get_ratio()
{
	return win_res.x / win_res.y;
}

OBJECT::TYPE camera::get_type()
{
    return OBJECT::TYPE::CAMERA;
}

SDL_Renderer *camera::get()
{
    return sdl_renderer;
}

void camera::connect_object(game_object* object)
{
	connected_object = object;
}

void camera::clear()
{
	clear(colors::BLACK);
}

void camera::clear(unsigned int color)
{
	clear(rgba(color));
}

void camera::clear(rgba color)
{
	set_color(color);
	
	//reset_viewport();
	SDL_RenderClear(sdl_renderer);
	//restore_viewport();

	set_color(colors::WHITE);
}

void camera::set_color(unsigned int color)
{
	set_color(rgba(color));
}

void camera::set_color(rgba color)
{
	SDL_SetRenderDrawColor(sdl_renderer, color.color.r, color.color.g, color.color.b, color.color.a);
}

void camera::present()
{
	SDL_RenderPresent(sdl_renderer);
}

void camera::set_viewport(SDL_FRect viewport)
{
	//pos = ;
	//size = { viewport.w, viewport.h };
	//viewport = { viewport.x, viewport.y,viewport.w, viewport.h  };
}

SDL_FRect camera::get_viewport()
{
	return viewport;
}

SDL_FRect* camera::get_viewport_ptr()
{
	return &viewport;
}

vec2 camera::get_mouse_relative_pos(float m_x, float m_y)
{
	auto res = get_pos();
	res.x = (-res.x + m_x);
	res.y = (-res.y + m_y);
	//std::cout << res.x << " " << res.y << std::endl;

	return res;
}

SDL_Texture* camera::get_scene()
{
	return scene;
}

void camera::draw_debug_text(std::string text, vec2 pos)
{
	if (show_gui) {
		SDL_RenderDebugText(sdl_renderer, pos.x, pos.y, text.c_str());
	}
}

void camera::draw_debug_info()
{
	if (show_gui) {
		draw_debug_text(std::to_string(fps::get()), vec2());

		std::string new_str;
		float y = 16.0f;
		for (auto ch = debug_text.begin(); ch != debug_text.end(); ch++) {
			if (*ch != '\n') {
				new_str += *ch;
			}
			else {
				draw_debug_text(new_str, vec2(0.0f, y));
				y += 16.0f;
				new_str.clear();
			}
		}
		
	}
}
