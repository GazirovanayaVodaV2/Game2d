#pragma once
#include "base.h"

#include <base_object.h>
#include <window.h>
#include <utils.h>

#include <texture.h>
#include <map.h>
#include "wall.h"

#include "GuiLib/gui_elements.h"

#include <map>
#include <list>
#include <vector>
#include <memory>

class game :
    public base
{
private:
	std::unique_ptr<atlas> txt_context;
	//std::shared_ptr<map> game_map;

	std::shared_ptr<game_object> last_selected_obj;

	gui::context Gui;

	vec2 last_raycasted_pos, pl_pos;

	bool pause = false, in_inventory = false;
	bool should_quit = false;
	//std::shared_ptr<animation::physic_animation_player> phys_player;
	//std::shared_ptr<wall> test_wall;

public:
	game();
	~game() override;

	SDL_AppResult cycle();
	SDL_AppResult input(const SDL_Event* event);

	gui::context& get_gui_context();

	OBJECT::TYPE get_type() override { return OBJECT::TYPE::GAME; }
};

