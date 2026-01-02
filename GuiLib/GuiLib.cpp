
// GuiLib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"
#include "GuiLib.h"

gui::page::~page()
{
	print::info("Deleting ui page");
	for (auto& el : elements) {
		delete el;
	}
}

void gui::page::update(float delta_time)
{
	if (active) {
		for (auto& element : elements) {
			element->update(delta_time);
		}
	}
}

void gui::page::draw(SDL_Renderer* render)
{
	if (active) {
		for (auto& element : elements) {
			element->draw(render);
		}
	}
}

void gui::page::input(const SDL_Event* event)
{
	if (active) {
		for (auto& element : elements) {
			element->input(event);
		}
	}
}

gui::page& gui::page::add(base* el)
{
	elements.emplace_back(el);
	return *this;	
}

size_t gui::page::size()
{
	return elements.size();
}

void gui::page::activate()
{
	active = true;

	SDL_Event empty_event{0};
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
	for (auto& [name, page] : pages) {
		delete page;
	}

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
	for (auto& [name, page] : pages) {
		page->draw(render);
	}

	SDL_SetRenderTarget(render, NULL);

	SDL_Rect rect;
	SDL_FRect dest;
	SDL_GetRenderViewport(render, &rect);

	dest = {
		(float)rect.x,
		(float)rect.y,
		(float)rect.w,
		(float)rect.h
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