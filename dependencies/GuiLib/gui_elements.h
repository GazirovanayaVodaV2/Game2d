#pragma once

#include "GuiLib.h"
#include <SDL3_ttf/SDL_ttf.h>

namespace gui {


	class color_rect : public base {
	public:
		SDL_Color fill_color{ 0 }, border_color{0};
		int border_width = 0;

		color_rect() = default;
		~color_rect() = default;

		void update(float delta_time) override {};
		void draw(SDL_Renderer* render) override;
		void input(const SDL_Event* event) override {};

		types get_type() override {
			return types::rect;
		}
	};

	class image_rect : public base {
	private:
		SDL_Texture* image = nullptr;

		void image_deleter();
	public:
		float scale_x = 1.0f, scale_y = 1.0f;

		image_rect() = default;
		image_rect(SDL_Renderer* render, std::string path_);
		~image_rect();

		void update(float delta_time) override {};
		void draw(SDL_Renderer* render) override;
		void input(const SDL_Event* event) override {};

		void set_image(SDL_Renderer* render, std::string path_);

		types get_type() override {
			return types::rect;
		}

		SDL_Texture* get_texture() {
			return image;
		}
	};

	//add text box
	class text_box : public base {
	private:
		TTF_Font* font = nullptr;
		
		SDL_Texture* txt = nullptr;

		SDL_Renderer* renderer_ptr;

		std::string text = "Sample Text";
		SDL_Color color = { 255 , 255, 255, 255};

		void text_creator();
		void text_deleter();
		void text_recreate();
	public:
		text_box() = default;
		~text_box();

		text_box(SDL_Renderer* render, std::string path_, std::string content, SDL_Color color);

		void allign(SDL_FRect parent_rect);
		void resize_parent(SDL_FRect* parent_rect, bool move_parent = false);

		void update(float delta_time) override {};
		void draw(SDL_Renderer* render) override;
		void input(const SDL_Event* event) override {};

		void set_text(std::string text);
		void set_color(SDL_Color color);

		types get_type() override {
			return types::text_box;
		}
	};

	class button_base : public base, public clickable_object {
	protected:
		//Shitty code
		SDL_FRect* viewport = nullptr;
	public:
		void input(const SDL_Event* event) override;
		void force_click();
		virtual void force_highlight(SDL_Renderer* render) = 0;

		types get_type() override {
			return types::button;
		}
	};


	class colored_button : public button_base {
	public:
		gui::color_rect rect, hover, clicked;

		colored_button() = default;
		~colored_button() = default;

		colored_button(SDL_FRect* viewport_ptr) {
			viewport = viewport_ptr;
		};

		void update(float delta_time) override {};
		void draw(SDL_Renderer* render) override;
		void force_highlight(SDL_Renderer* render) override;
	};

	class button : public colored_button {
	public:
		gui::text_box* label = nullptr;

		button(SDL_FRect* viewport_ptr)
			: colored_button(viewport_ptr)
		{
		};

		~button();

		void draw(SDL_Renderer* render) override;
	};
}