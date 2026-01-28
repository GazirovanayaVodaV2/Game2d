#include "headers/inventory.h"

#include "GuiLib/gui_elements.h"
#include "headers/window.h"

void inventory::inventory::try_place_selected_item(int x, int y)
{
	if (selected_item_id != -1) {
		int id = x + y * w;
		if (!items.at(id)) {
			items[id] = items[selected_item_id];
			items[selected_item_id] = nullptr;

			if (selected_item_id == item_in_hands_id) {
				item_in_hands_id = id;
			}

			selected_item_id = -1;
		}
	}
}

inventory::inventory::inventory(entity* parent)
	: inv_gui(camera::get())
{
	this->parent = parent;
	resize(w, h);
}

inventory::inventory::~inventory()
{
	print::info("Deleting inventory");
}


void inventory::inventory::resize(int w_, int h_)
{
	w = w_;
	h = h_;
	items.resize(w_ * h_);

	if (inv_gui.size() > 0) {
		delete inv_gui["inv_page"];
	}

	lights.clear();
	
	auto inv_page = new gui::page();

	for (int y = cell_margin; y < h * (cell_size + cell_margin); y += cell_size + cell_margin) {
		for (int x = cell_margin; x < w * (cell_size + cell_margin); x += cell_size + cell_margin) {
			auto cell = new gui::color_rect();
			cell->box = { (float)x, (float)y, (float)cell_size, (float)cell_size };
			cell->fill_color = { 127, 127, 127, 255 };

			inv_page->add(cell);

			lights.add_light("textures\\lightsource.png", 0.35f, 0xfffffffff);
			auto pos = vec2((float)x + cell_size / 2, (float)y + cell_size / 2);
			lights.get_last()->set_pos(pos);
		}
	}

	auto hp_img = new gui::image_rect(camera::get(), "textures\\inventory_hp.png");

	hp_img->box.x = h * (cell_size + cell_margin);
	inv_page->add(hp_img);

	inv_hp_text_pos = vec2(hp_img->box.x, hp_img->box.y);
	inv_hp_text_size = vec2(hp_img->box.w, hp_img->box.h);

	lights.add_light("textures\\lightsource.png", 1.0f, 0xF09B59);
	auto pos = vec2(hp_img->box.x, 0.0f);
	lights.get_last()->set_pos(pos);

	inv_gui.add("inv_page", inv_page);
}

bool inventory::inventory::try_add_item(item* item_)
{
	if (item_->is_pickable() and !item_->in_inventory()) {
		for (size_t i = 0; i < items.size(); i++) {
			if (!items[i]) {
				item_->pick_up();
				items[i] = item_;
				return true;
			}
		}
	}

	return false;
}

void inventory::inventory::try_use_item()
{
	auto it = get_item(cursor_x, cursor_y);
	if (it) {
		if (it->can_hold_in_hands()) {
			item_in_hands = it;
			item_in_hands_id = cursor_x + cursor_y * w;
		}
		else {
			it->use(parent);
			remove_item(cursor_x, cursor_y);
		}

		update(1.0f);
	}
}

inventory::item* inventory::inventory::get_item(int x, int y)
{
	return items.at(x + y * w);
}

inventory::item* inventory::inventory::get_item_in_hands()
{
	return item_in_hands;
}

void inventory::inventory::remove_item(int x, int y)
{
	int i = x + y * w;
	items[i] = nullptr;

	if (selected_item_id == i) {
		item_in_hands = nullptr;
	}
}

void inventory::inventory::draw()
{
	if (opened) {
		inv_gui.draw(camera::get());

		SDL_Color render_color;

		SDL_GetRenderDrawColor(camera::get(),
			&render_color.r,
			&render_color.g,
			&render_color.b,
			&render_color.a);

		
		SDL_SetRenderDrawColor(camera::get(), 210, 210, 210, 255);
		if (selected_item_id != -1) {
			SDL_SetRenderDrawColor(camera::get(), 255, 86, 68, 255);
		}
		SDL_FRect cursor = { cursor_x * (cell_size + cell_margin) + cell_margin,
			cursor_y* (cell_size + cell_margin) + cell_margin,
			cell_size,cell_size };
		SDL_RenderFillRect(camera::get(), &cursor);


		if (item_in_hands_id != -1) {
			SDL_SetRenderDrawColor(camera::get(), 31, 163, 70, 255);

			SDL_FRect item_in_hands_rect = { (item_in_hands_id % w) * (cell_size + cell_margin) + cell_margin,
							(item_in_hands_id / w)* (cell_size + cell_margin) + cell_margin,
							cell_size,cell_size };
			SDL_RenderFillRect(camera::get(), &item_in_hands_rect);
		}
		

		for (int i = 0; i < items.size(); i++) {
			auto& it = items[i];


			if (i == selected_item_id) {
				auto items_pos = vec2(cursor_x * (cell_size + cell_margin) + cell_margin * 2,
					cursor_y * (cell_size + cell_margin) + cell_margin * 2) - camera::get_pos();
				it->set_pos(items_pos);
			}
			if(it) it->draw();
		}

		SDL_SetRenderDrawColor(camera::get(),
			render_color.r,
			render_color.g,
			render_color.b,
			render_color.a);


		lights.draw();
	}
}

void inventory::inventory::update(float delta)
{
	int i = 0;
	for (int y = cell_margin; y < h * (cell_size + cell_margin); y += cell_size + cell_margin) {
		for (int x = cell_margin; x < w * (cell_size + cell_margin); x += cell_size + cell_margin) {
			auto pos_ = vec2((float)x + cell_margin, (float)y + cell_margin) - camera::get_pos();
			auto size_ = vec2((float)cell_size, (float)cell_size);
			if (items[i]) {
				auto& item_ = items[i]; 
				if (i != selected_item_id) {
					item_->set_pos(pos_);
				}
				item_->set_size(size_);
			}

			pos_ = vec2((float)x + (cell_size / 2.0f), (float)y + (cell_size / 2.0f)) - camera::get_pos();
			lights.get_light(i)->set_pos(pos_);

			i++;
		}
	}

	auto p = inv_hp_text_pos + (inv_hp_text_size / 2) - camera::get_pos();
	p.y -= 200;
	lights.get_light(w * h)->set_pos(p);
	auto clr = rgba(convert::f2i(255 - (255 * (parent->get_hp() / 100.0f))), 
					convert::f2i(255 * (parent->get_hp() / 100.0f)), 0);
	lights.get_light(w * h)->set_color(clr.to_hex());
}

bool inventory::inventory::input(const SDL_Event* event)
{
	if (event->type == SDL_EVENT_KEY_DOWN) {
		switch (event->key.key)
		{
		case SDLK_E: {
			if (opened) {
				close();
			} else {
				open();
			}
		} break;
		case SDLK_ESCAPE: {
			opened = false;
			inv_gui.deactivate();
		} break;
		default: {
		} break;
		}

		if (opened) {
			switch (event->key.key) {
			case SDLK_W: {
				if (cursor_y > 0) {
					cursor_y--;
				}
			} break;
			case SDLK_S: {
				if (cursor_y < h - 1) {
					cursor_y++;
				}
			} break;
			case SDLK_D: {
				if (cursor_x < w - 1) {
					cursor_x++;
				}
			} break;
			case SDLK_A: {
				if (cursor_x > 0) {
					cursor_x--;
				}
			} break;
			case SDLK_RETURN: {
				if (selected_item_id == -1) {
					try_use_item();
				}
				else {
					try_place_selected_item(cursor_x, cursor_y);
				}
			} break;
			case SDLK_BACKSPACE: {
				int id = cursor_x + cursor_y * w;
				auto& it = items[id];
				if (it) {
					selected_item_id = id;
				}
			}break;
			default: {
			} break;
			}
		}
	}
	else if (event->type == SDL_EVENT_KEY_UP) {
		switch (event->key.key)
		{
			default:
			{
			} break;
		}
	}
	return opened;
}

void inventory::inventory::open()
{
	opened = true;
	inv_gui.activate();
}

void inventory::inventory::close()
{
	opened = false;
	inv_gui.deactivate();
}

bool inventory::inventory::is_opened()
{
	return opened;
}
