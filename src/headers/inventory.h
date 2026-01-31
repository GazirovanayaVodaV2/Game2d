#pragma once

#include "interactive_object.h"
#include "entity.h"
#include "light_system.h"
#include "GuiLib/GuiLib.h"

#define DISABLE_INVENTORY_LIGHTS

namespace inventory {
	using item = interactive_object_base;
	class inventory {
	private:
		gui::context inv_gui;
#ifndef DISABLE_INVENTORY_LIGHTS
		light::system lights;
#endif
		int w = 5, h = 5;
		const int cell_margin = 10;
		const int cell_size = 86;
		int cursor_x = 0, cursor_y = 0;
		std::vector<item*> items{nullptr};
		item* item_in_hands = nullptr;
		int item_in_hands_id = -1;
		int selected_item_id = -1;

		entity* parent = nullptr;

		bool opened = false;

		vec2 inv_hp_text_pos, inv_hp_text_size;

		void try_place_selected_item(int x, int y);
	public:
		inventory(entity* parent);
		~inventory();

		void draw();
		void resize(int w_, int h_);

		/// <summary>
		/// Trying to add item in inventory. In success return TRUE
		/// </summary>
		/// <param name="item_"></param>
		/// <returns></returns>
		bool try_add_item(item* item_);
		
		void try_use_item();

		item* get_item(int x, int y);
		item* get_item_in_hands();

		item* find_item(const std::type_info& type);

		void update(float delta);

		void remove_item(size_t i);
		void remove_item(int x, int y);		
		void remove_item(const std::type_info& type);
		/// <summary>
		/// Returns TRUE is inventory opened
		/// </summary>
		/// <param name="event"></param>
		/// <returns></returns>
		bool input(const SDL_Event* event);

		void open();
		void close();
		bool is_opened();
	};
}



