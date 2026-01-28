
// GuiLib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"
#include "GuiLib.h"
#include "gui_elements.h"
#include <SDL3/SDL_keycode.h>

gui::page::~page()
{
	print::info("Deleting ui page");
	for (auto& [name, element] : elements) {
		delete element;
	}
	elements.clear();
	elements_names.clear();
}

void gui::page::update(float delta_time)
{
	if (active) {
		for (auto& [name, element] : elements) {
			element->update(delta_time);
		}
	}
}

void gui::page::draw(SDL_Renderer* render)
{
	if (active) {
		for (auto& [name, element] : elements) {
			element->draw(render);
		}

		if (enable_keyboard_selection) {
			auto& el_name = elements_names.at(keyboard_selected_item);
			auto item = elements.at(el_name);
			if (item->get_type() == gui::types::button) {
				auto button = (button_base*)item;
				button->force_highlight(render);
			}
		}
	}
}

void gui::page::input(const SDL_Event* event)
{
	if (active) {
		for (auto& [name, element] : elements) {
			element->input(event);
		}

		if (event->type == SDL_EVENT_KEY_DOWN) {
			
			if (enable_keyboard_selection) {
				switch (event->key.key)
				{
					case SDLK_DOWN: {
						if (keyboard_selected_item < elements.size() - 1) {
							keyboard_selected_item++;
						}
					} break;
					case SDLK_UP: {
						if (keyboard_selected_item > 0) {
							keyboard_selected_item--;
						}
					} break;
					case SDLK_RETURN: {
						auto& el_name = elements_names.at(keyboard_selected_item);
						auto item = elements.at(el_name);
						if (item->get_type() == gui::types::button) {
							auto button = (button_base*)item;
							button->force_click();
						}
					} break;
					default: {
					} break;
				}
			}

			switch (event->key.key)
			{
				case SDLK_DOWN: {
					enable_keyboard_selection = true;
				} break;
				case SDLK_UP: {
					enable_keyboard_selection = true;
				} break;
			}
			
		}
		else if (event->type == SDL_EVENT_MOUSE_MOTION) {
			enable_keyboard_selection = false;
		}
	}
}

gui::page& gui::page::add(base* el, std::string name)
{
	auto res_name = name;
	if (name.empty()) {
		constexpr int safe_limit = 1024;
		for (int int_name = 0; int_name < safe_limit; int_name++) {
			std::string new_name = std::to_string(int_name);

			if (elements.find(new_name) == elements.end()) {
				res_name = new_name;

				goto add;
			}
		}

		print::error("Failed to add unnamed gui object!");
	}

	add:
	elements[res_name] = el;
	elements_names[elements.size()-1] = res_name;
	return *this;	
}

gui::base* gui::page::get(std::string name)
{
	return elements.at(name);
}

size_t gui::page::size()
{
	return elements.size();
}

void gui::page::activate()
{
	active = true;

	SDL_Event empty_event{0};
	empty_event.type = SDL_EVENT_MOUSE_MOTION;
	input(&empty_event);
}

void gui::page::deactivate()
{
	active = false;
}

bool gui::page::is_active()
{
	return active;
}

gui::context::context(SDL_Renderer* renderer)
{
	target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
									1000, 1000);
	SDL_SetTextureScaleMode(target, SDL_SCALEMODE_NEAREST);
}

gui::context::~context()
{
	print::info("Deleting gui context");
	print::increase_level();
	for (auto& [name, page] : pages) {
		delete page;
	}
	print::decrease_level();
	SDL_DestroyTexture(target);
}

void gui::context::update(float delta_time)
{
	for (auto& [name, page] : pages) {
		page->update(delta_time);
	}
}

void gui::context::draw(SDL_Renderer* render)
{

	SDL_SetRenderTarget(render, target);

	SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
	SDL_RenderClear(render);
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);

	if (SDL_GetRenderTarget(render) != target) {
		print::warning("Failed to set target in GUI!");
	}

	for (auto& [name, page] : pages) {
		page->draw(render);
	}

	SDL_SetRenderTarget(render, NULL);

	SDL_Rect viewport;
	SDL_GetRenderViewport(render, &viewport);

	/*auto delta_w = (viewport.w - 1000.0f) / 2.0f;
	auto delta_h = (viewport.h - 1000.0f) / 2.0f;

	SDL_FRect dest = {
		delta_w,
		delta_h,
		viewport.w / (viewport.w / 1000.0f),
		viewport.h / (viewport.h / 1000.0f)
	};*/

	SDL_FRect dest = {
	0,
	0,
	viewport.w / (viewport.w / 1000.0f),
	viewport.h / (viewport.h / 1000.0f)
	};

	SDL_RenderTexture(render, target, NULL, &dest);
}

void gui::context::input(const SDL_Event* event)
{
	for (auto& [name, page] : pages) {
		page->input(event);
	}
}

gui::context& gui::context::add(std::string name, page* pg)
{
	pages[name] = (pg);
	return *this;
}

gui::page* gui::context::operator[](std::string name)
{
	return pages.at(name);
}

size_t gui::context::size()
{
	return pages.size();
}

void gui::context::activate()
{
	for (auto& [name, page] : pages) {
		page->activate();
	}
}

void gui::context::deactivate()
{
	for (auto& [name, page] : pages) {
		page->deactivate();
	}
}
