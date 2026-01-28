#include "gui_elements.h"
#include "gui_elements.h"
#include "gui_elements.h"
#include "gui_elements.h"
#include "gui_elements.h"
#include "gui_elements.h"
#include "gui_elements.h"
#include "gui_elements.h"
#include "gui_elements.h"
#include "pch.h"
#include "gui_elements.h"

void gui::color_rect::draw(SDL_Renderer* render)
{
	SDL_Color render_color;

	SDL_GetRenderDrawColor(render,
		&render_color.r,
		&render_color.g,
		&render_color.b,
		&render_color.a);

	//Draw border
	SDL_SetRenderDrawColor(render,
		border_color.r,
		border_color.g,
		border_color.b,
		border_color.a);
	SDL_RenderFillRect(render, &box);

	//Fill rect
	SDL_SetRenderDrawColor(render,
		fill_color.r,
		fill_color.g,
		fill_color.b,
		fill_color.a);
		box.x += border_width;
		box.y += border_width;
		box.w -= border_width * 2;
		box.h -= border_width * 2;
		SDL_RenderFillRect(render, &box);
}

void gui::image_rect::image_deleter()
{
	print::info("Deleting gui image");
	if (image) {
		SDL_DestroyTexture(image);
	}
}

gui::image_rect::image_rect(SDL_Renderer* render, std::string path_)
{
	set_image(render, path_);
}

gui::image_rect::~image_rect()
{
	image_deleter();
}

void gui::image_rect::draw(SDL_Renderer* render)
{
	if (image) {
		SDL_FRect rect = box;

		if ((rect.w == 0) or (rect.h == 0)) {
			SDL_GetTextureSize(image, &rect.w, &rect.h);
		}

		rect.w *= scale_x;
		rect.h *= scale_y;

		SDL_RenderTexture(render, image, NULL, &rect);
	}
}

void gui::image_rect::set_image(SDL_Renderer* render, std::string path_)
{
	image_deleter();

	print::loading(path_);
	image = IMG_LoadTexture(render, path(path_).c_str());
	if (!image) {
		auto err_str = std::format("Something wrong with image_rect. Path: {} Log: {} Error Code: {}",
			path(path_), SDL_GetError(), static_cast<int>(SDL_APP_FAILURE));

		print::error(err_str);
		throw err_str.c_str();
	}

	SDL_GetTextureSize(image, &box.w, &box.h);

	print::loaded();
}


void gui::button_base::input(const SDL_Event* event) {
	if (event->type == SDL_EVENT_MOUSE_MOTION) {
		float x, y;
		SDL_GetMouseState(&x, &y);

		//Shitty code
		/*if (viewport) {
			x /= (viewport->w / 1366);
			y /= (viewport->h / 768);
		}*/

		mouse_hover = (((x > box.x) and (x < (box.x + box.w))) and
			((y > box.y) and (y < (box.y + box.h))));

		left_click = false;
		right_click = false;
	}

	if (mouse_hover) {
		if (on_mouse_hover) {
			on_mouse_hover();
		}

		if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			left_click = event->button.button == SDL_BUTTON_LEFT;
			right_click = event->button.button == SDL_BUTTON_RIGHT;
			if (left_click) {
				if (on_mouse_left_click) {
					on_mouse_left_click();
				}
			}

			if (right_click) {
				if (on_mouse_right_click) {
					on_mouse_right_click();
				}
			}
		}
	}
}

void gui::button_base::force_click()
{
	on_mouse_left_click();
}



void gui::colored_button::draw(SDL_Renderer* render) {
	rect.box = box;
	hover.box = box;
	clicked.box = box;

	rect.draw(render);
	if (mouse_hover) {
		hover.draw(render);
	}
	if (left_click or right_click) {
		clicked.draw(render);
	}
}

void gui::colored_button::force_highlight(SDL_Renderer* render)
{
	hover.draw(render);
}

static SDL_Texture* text_creator_and_color_setter_impl(TTF_Font* font, SDL_Renderer* render,
													  std::string content, SDL_Color color) {
	auto surf = TTF_RenderText_Blended(font, content.c_str(), content.size(), color);
	auto res = SDL_CreateTextureFromSurface(render, surf);
	SDL_DestroySurface(surf);
	return res;
}

void gui::text_box::text_creator()
{
	txt = text_creator_and_color_setter_impl(font, renderer_ptr, text, color);
}

void gui::text_box::text_deleter()
{
	if (txt)
		SDL_DestroyTexture(txt);
}

void gui::text_box::text_recreate()
{
	text_deleter();
	text_creator();
}

gui::text_box::~text_box()
{

	if (font)
	{
		TTF_CloseFont(font);
		font = nullptr;
	}
		

	text_deleter();
}

gui::text_box::text_box(SDL_Renderer* render, std::string path_, std::string content, SDL_Color color)
{
	renderer_ptr = render;
	auto path__ = path(path_);
	//font = TTF_OpenFont(path__.c_str(), path__.size());
	font = TTF_OpenFont(path__.c_str(), 64);

	if (!font) {
		throw std::format("Failed to open font: {}", path__).c_str();
	}

	//TODO:
		//somehow make posible dynamic color change
	text = content;
	this->color = color;
	text_creator();
}

void gui::text_box::allign(SDL_FRect parent_rect)
{
	box = parent_rect;

	SDL_GetTextureSize(txt, &box.w, &box.h);
}

void gui::text_box::resize_parent(SDL_FRect* parent_rect, bool move_parent)
{
	SDL_FRect diff{0};
	diff.x = parent_rect->x - this->box.x;
	diff.y = parent_rect->y - this->box.y;
	diff.w = parent_rect->w - this->box.w;
	diff.h = parent_rect->h - this->box.h;

	*parent_rect = this->box;

	if (move_parent) {
		parent_rect->x += diff.w / 2;
		this->box.x += diff.w / 2;
	}
}

void gui::text_box::draw(SDL_Renderer* render)
{
	if (txt) {
		SDL_Color render_color;

		SDL_GetRenderDrawColor(render,
								&render_color.r,
								&render_color.g,
								&render_color.b,
								&render_color.a);

		//auto rect = calc_rect(box, viewport);
		SDL_RenderTexture(render, txt, NULL, &box);

		SDL_SetRenderDrawColor(render,
								render_color.r,
								render_color.g,
								render_color.b,
								render_color.a);
	}
}

void gui::text_box::set_text(std::string text)
{
	this->text = text;
	text_recreate();
}

void gui::text_box::set_color(SDL_Color color)
{
	text_recreate();
}


gui::button::~button()
{
	delete label;
}

void gui::button::draw(SDL_Renderer* render) {
	gui::colored_button::draw(render);

	if (label) {
		label->draw(render);
	}
}