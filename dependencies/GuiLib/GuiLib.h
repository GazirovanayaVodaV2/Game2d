#pragma once

#include "pch.h"


namespace gui {
	enum class types {
		page,
		context,
		rect,
		text_box,
		button
	};

	class Igui {
	public:
		Igui() = default;
		virtual ~Igui() = default;

		virtual void update(float delta_time) = 0;
		virtual void draw(SDL_Renderer* render) = 0;
		virtual void input(const SDL_Event* event) = 0;
		virtual types get_type() = 0;
	};

	class clickable_object {
	protected:
		bool mouse_hover = false, left_click = false, right_click = false;
	public:
		std::function<void()> on_mouse_left_click, on_mouse_right_click, on_mouse_hover;
	};

	class base : public Igui {
	public:
		SDL_FRect box{ 0 };

		base() = default;
		~base() = default;
	};

	class page : public Igui {
	private:
		std::map<std::string, base*> elements;
		std::map<size_t, std::string> elements_names;

		size_t keyboard_selected_item = 0;
		bool enable_keyboard_selection = false;

		bool active = false;
	public:
		page() = default;
		~page();

		void update(float delta_time) override;
		void draw(SDL_Renderer* render) override;
		void input(const SDL_Event* event) override;

		/// <summary>
		/// It's important that the argument be a pointer created with NEW, not an address. Page will then clear all elements 
		/// </summary>
		/// <param name="el"></param>
		/// <returns></returns>
		page& add(base* el, std::string name = "");

		base* get(std::string name);

		size_t size();

		void activate();
		void deactivate();
		bool is_active();

		types get_type() override {
			return types::page;
		}
	};

	class context : public Igui {
	private:
		std::map<std::string, page*> pages;
		SDL_Texture* target = nullptr;
	public:
		context() = delete;
		context(SDL_Renderer* renderer);
		~context();

		void update(float delta_time) override;
		void draw(SDL_Renderer* render) override;
		void input(const SDL_Event* event) override;

		/// <summary>
		/// It's important that the argument be a pointer created with NEW, not an address. Context will then clear all pages 
		/// </summary>
		/// <param name="el"></param>
		/// <returns></returns>
		context& add(std::string name, page* pg);

		page* operator[](std::string name);
		size_t size();

		void activate();
		void deactivate();

		types get_type() override {
			return types::context;
		}
	};
};