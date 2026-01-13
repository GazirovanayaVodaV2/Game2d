#pragma once

#include "interactive_object.h"
#include "entity.h"
#include "light_system.h"
#include "GuiLib/GuiLib.h"

namespace inventory {
	using item = interactive_object_base;
	class inventory {
	private:
		gui::context inv_gui;
		light::system lights;

		int w = 5, h = 5;
		const int cell_margin = 10;
		const int cell_size = 86;
		int cursor_x = 0, cursor_y = 0;
		std::vector<std::shared_ptr<item>> items{nullptr};
		int selected_item_id = -1;

		entity* parent = nullptr;

		bool opened = false;

		vec2 inv_hp_text_pos, inv_hp_text_size;

		void try_place_selected_item(int x, int y);
		void remove_item(int x, int y);
	public:
		inventory(entity* parent);
		~inventory() = default;

		void draw();
		void resize(int w_, int h_);

		/// <summary>
		/// Trying to add item in inventory. In success return TRUE
		/// </summary>
		/// <param name="item_"></param>
		/// <returns></returns>
		bool try_add_item(std::shared_ptr<item> item_);
		
		void try_use_item();

		std::shared_ptr<item> get_item(int x, int y);

		void update(float delta);

		/// <summary>
		/// Returns TRUE is inventory opened
		/// </summary>
		/// <param name="event"></param>
		/// <returns></returns>
		bool input(const SDL_Event* event);
		bool is_opened();
	};
}



