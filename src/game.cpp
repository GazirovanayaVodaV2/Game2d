#include "game.h"

#include <memory>
#include <iostream>

#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_timer.h"

#include <utils.h>
#include <texture.h>
#include <entity.h>
#include "wall.h"
#include "animation.h"

game::game()
{
	print::loading("Starting game");
	print::increase_level();

	auto renderer = camera::get();

	txt_context = std::make_shared<atlas>();

	level_manager::add("maps\\test.level", txt_context);

	//main menu
	auto main_menu = new gui::page();

	auto new_game = new gui::button(camera::get_viewport_ptr());
	new_game->box = { 10, 200, 300, 100 };
	new_game->rect.fill_color = { 255, 255, 255, 255 };
	new_game->hover.fill_color = { 255, 0, 0, 255 };
	new_game->clicked.fill_color = { 0, 0, 0, 255 };
	new_game->on_mouse_left_click = [this, main_menu]() {
		level_manager::load("first"); //change later
		camera::connect_object(level_manager::get()->get_player());
		main_menu->deactivate();
		};
	new_game->label = new gui::text_box(&renderer, "fonts\\NotoSans.ttf", "New game", { 0 });
	new_game->label->allign(new_game->box);
	
	main_menu->add(new_game);

	auto main_menu_quit = new gui::button(camera::get_viewport_ptr());
	main_menu_quit->box = { 10, 350, 300, 100 };
	main_menu_quit->rect.fill_color = { 255, 255, 255, 255 };
	main_menu_quit->hover.fill_color = { 255, 0, 0, 255 };
	main_menu_quit->clicked.fill_color = { 0, 0, 0, 255 };
	main_menu_quit->on_mouse_left_click = [this, main_menu]() {
		exit(0);
		};
	main_menu_quit->label = new gui::text_box(&renderer, "fonts\\NotoSans.ttf", "Quit", { 0 });
	main_menu_quit->label->allign(main_menu_quit->box);

	main_menu->add(main_menu_quit);
	main_menu->activate();


	//Ingame pause
	auto pause_page = new gui::page();
	auto resume_button = new gui::button(camera::get_viewport_ptr());
	resume_button->box = { 500, 100, 400, 100 };
	resume_button->rect.fill_color = { 255, 255, 255, 255 };
	resume_button->hover.fill_color = { 255, 0, 0, 255 };
	resume_button->clicked.fill_color = { 0, 0, 0, 255 };

	resume_button->on_mouse_left_click = [this, pause_page]() {
		this->pause = false;
		pause_page->deactivate();
		};

	resume_button->label = new gui::text_box(&renderer, "fonts\\NotoSans.ttf", "Resume", {0});
	resume_button->label->allign(resume_button->box);
	pause_page->add(resume_button);

	auto quit_to_main_menu_button = new gui::button(camera::get_viewport_ptr());
	quit_to_main_menu_button->box = { 500, 250, 400, 100 };
	quit_to_main_menu_button->rect.fill_color = { 255, 255, 255, 255 };
	quit_to_main_menu_button->hover.fill_color = { 255, 0, 0, 255 };
	quit_to_main_menu_button->clicked.fill_color = { 0, 0, 0, 255 };

	quit_to_main_menu_button->on_mouse_left_click = [this, pause_page]() {
		this->pause = false;
		Gui["main_menu"]->activate();

		level_manager::unload_all();

		pause_page->deactivate();
		};

	quit_to_main_menu_button->label = new gui::text_box(&renderer, "fonts\\NotoSans.ttf", "Quit to main menu", { 0 });
	quit_to_main_menu_button->label->allign(quit_to_main_menu_button->box);
	pause_page->add(quit_to_main_menu_button);


	Gui.add("pause", pause_page);
	Gui.add("main_menu", main_menu);

	//test_wall = std::make_shared<wall>(txt_context->get("brick"));
	//phys_player = std::make_shared<animation::physic_animation_player>();
	//phys_player->add(new animation::linear(100, 100, 0, 1000));

	print::decrease_level();
	print::loaded("Game loaded");
}

game::~game()
{
	print::info("Closing game");

}

SDL_AppResult game::cycle()
{
	auto delta = fps::delta();
	camera::clear();
	
	level_manager::draw();

	/* Draw UI */

	camera::reset_viewport();

	camera::draw_debug_text(std::to_string(fps::get()), vec2());

	camera::restore_viewport();

	//auto trans = phys_player->update(convert::ns2ms(fps::.delta()), {test_wall->get_pos(), 
	//																	   test_wall->get_angle()});

	//test_wall->move_on(trans.pos);
	//test_wall->rotate(trans.angle);

	//test_wall->draw(cam->get(), cam->get_viewport());

	/*
	End of rendering
	*/

	Gui.draw(camera::get());

	camera::present();

	camera::set_viewport({ 0,0, window::get_size().x(), window::get_size().y()});

	if (not pause) {
		level_manager::update(delta);
	}

	camera::update(delta);
	fps::update();
	if (window::get_fps() != -1) {
		SDL_DelayNS(static_cast<Uint64>(convert::f2i(
			convert::fps2delta(static_cast<float>(window::get_fps()))
			)));
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult game::input(const SDL_Event* event)
{

	level_manager::input(event);
	
	Gui.input(event);

	camera::input(event);
	auto res = window::input(event);
	if (event->type == SDL_EVENT_WINDOW_RESIZED) {

		auto win_size = window::get_size();

		SDL_SetRenderLogicalPresentation(camera::get(), 
			win_size.x(), win_size.y(),
			SDL_LOGICAL_PRESENTATION_LETTERBOX);
	} else if (event->type == SDL_EVENT_KEY_DOWN) {
		if (event->key.key == SDLK_F1) {
			level_manager::load("first");
			camera::connect_object(level_manager::get()->get_player());

			Gui["main_menu"]->deactivate();
		}
		if (event->key.key == SDLK_F2) {
			level_manager::unload_all();
		}

		if (event->key.key == SDLK_ESCAPE) {
			if (!Gui["main_menu"]->is_active())
			{
				if (pause) {
					Gui["pause"]->deactivate();
					pause = false;
				}
				else {
					Gui["pause"]->activate();
					pause = true;
				}
			}
		}

		if (event->key.key == SDLK_E) {
			if (last_selected_obj) {
				if (last_selected_obj->get_type() == OBJECT::TYPE::WALL) {
					//auto new_obj = std::make_shared<wall>(*last_selected_obj);
					//level_manager::get()->add(new_obj);
				}
			}
		}
	}
#ifdef _DEBUG
	else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		float m_x, m_y;
		SDL_GetMouseState(&m_x, &m_y);
		auto rel_pos = camera::get_mouse_relative_pos(m_x, m_y);
		if (!level_manager::is_level_empty()) {
			auto& obj = level_manager::get()->get(rel_pos);

			if (!obj->is_null()) {
				if (obj->is_selected()) {
					obj->reject();
					last_selected_obj = nullptr;
				}
				else {
					obj->select();
					last_selected_obj = obj;
				}
			}
			else {
				if (last_selected_obj) {
					last_selected_obj->reject();
					last_selected_obj = nullptr;
				}
			}
		}
	}
#endif
	return res;
}

OBJECT::TYPE game::get_type()
{
	return type;
}
