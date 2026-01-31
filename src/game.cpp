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

#include "raycast.h"

game::game()
	: Gui(camera::get())
{
	print::loading("Starting game");
	print::increase_level();

	auto renderer = camera::get();

	bench::add("gui_draw");
	bench::add("game_update");
	bench::add("input");
	bench::add("game_draw");

#ifdef _DEBUG
	bench::enable();
#endif

	txt_context = std::make_unique<atlas>(camera::get());
	level_manager::set_atlas(txt_context.get());
	level_manager::add("maps\\test.level");

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
	new_game->label = new gui::text_box(renderer, "fonts\\NotoSans.ttf", "New game", { 0 });
	new_game->label->allign(new_game->box);
	
	main_menu->add(new_game);

	auto main_menu_quit = new gui::button(camera::get_viewport_ptr());
	main_menu_quit->box = { 10, 350, 300, 100 };
	main_menu_quit->rect.fill_color = { 255, 255, 255, 255 };
	main_menu_quit->hover.fill_color = { 255, 0, 0, 255 };
	main_menu_quit->clicked.fill_color = { 0, 0, 0, 255 };
	main_menu_quit->on_mouse_left_click = [this, main_menu]() {
		should_quit = true;
		};
	main_menu_quit->label = new gui::text_box(renderer, "fonts\\NotoSans.ttf", "Quit", { 0 });
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

	resume_button->label = new gui::text_box(renderer, "fonts\\NotoSans.ttf", "Resume", {0});
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

	quit_to_main_menu_button->label = new gui::text_box(renderer, "fonts\\NotoSans.ttf", "Quit to main menu", { 0 });
	quit_to_main_menu_button->label->allign(quit_to_main_menu_button->box);
	quit_to_main_menu_button->label->resize_parent(&quit_to_main_menu_button->box, true);

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

	
	bench::get("game_draw").start();
	camera::clear();
	
	level_manager::draw();

	/* Draw UI */

//	camera::reset_viewport();
	camera::draw_debug_info();

//	camera::restore_viewport();
	camera::reset_scale();
	bench::get("game_draw").stop();

	bench::get("gui_draw").start();
	Gui.draw(camera::get());
	/*
	
	We should draw players inventory separetly because lightning 

	*/
	if (level_manager::is_any_level_loaded()) {
		auto& inv = level_manager::get()->get_player()->get_inventory();
		inv.draw();
		in_inventory = inv.is_opened();

		if (pause and in_inventory) {
			inv.close();
		}
	}

	bench::get("gui_draw").stop();
/*#ifdef _DEBUG
	SDL_RenderLine(camera::get(), camera::get_viewport().w / 2,
		camera::get_viewport().h / 2,
		last_raycasted_pos.x + camera::get_viewport().x,
		last_raycasted_pos.y + camera::get_viewport().y);
#endif // _DEBUG*/

	camera::present();

	//camera::set_viewport({ 0,0, window::get_size().x, window::get_size().y});

	bench::get("game_update").start();
	if (!pause and !in_inventory) {
		level_manager::update(delta);
	}

	camera::update(delta);
	fps::update();
	bench::get("game_update").stop();

	
	bench::stop();
	camera::debug_text = bench::get_info_str(true);

	if (window::get_fps() != -1) {
		SDL_DelayNS((Uint64)(convert::f2i(
			convert::fps2delta((float)(window::get_fps()))
			)));
	}
	bench::start();
	//std::cout << bench::get_info(true);
	
	if (should_quit) {
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult game::input(const SDL_Event* event)
{
	bench::get("input").start();
	level_manager::input(event);
	
	Gui.input(event);

	camera::input(event);
	auto res = window::input(event);
	if (event->type == SDL_EVENT_KEY_DOWN) {
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
				if (pause and !in_inventory) {
					Gui["pause"]->deactivate();
					pause = false;
				}
				else {
					Gui["pause"]->activate();
					pause = true;
				}
			}
		}
	}

	bench::get("input").stop();
	return res;
}

gui::context& game::get_gui_context()
{
	return Gui;
}
