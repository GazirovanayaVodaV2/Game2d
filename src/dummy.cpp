#include "dummy.h"

#include "window.h"

void dummy::update_hp_box()
{
	this->hp_box->set_text(std::to_string(this->hp));
}

void dummy::death()
{
	entity::death();
	update_hp_box();
}

dummy::dummy(std::shared_ptr<atlas> atl, int hp)
{
	this->hp.set_new_defaults(hp);

	hp_box = new gui::text_box(camera::get(), "fonts\\NotoSans.ttf", "0", {255});

	txt = atl->get("dummy");

	this->size = txt->get_size();

	this->size = this->size / 2;

	update_hp_box();
}

dummy::~dummy()
{
	delete hp_box; 
}

void dummy::draw()
{

	auto txt_saved_pos = txt->get_pos();
	auto txt_saved_size = txt->get_size();

	txt->set_pos(pos);
	txt->set_size(size);

	txt->draw();

	auto new_pos = txt->get_pos() + camera::get_pos();
	hp_box->box = { new_pos.x, new_pos.y - 64, txt->get_size().x, 64};
	hp_box->draw(camera::get());

	txt->set_pos(txt_saved_pos);
	txt->set_size(txt_saved_size);
}

SDL_AppResult dummy::update(float delta_time)
{
	if (collided) {
		pos.y -= 1;
	}
	else {
		pos.y += 0.1;
	}

	clear_collision_buffer();
	return SDL_APP_CONTINUE;
}

bool dummy::check_collision(game_object* object)
{
	if (global_check_collision(object, this)) {
		collided_objects.push_back(object);
	}
	collided = collided_objects.size() > 0;

	return collided;
}

void dummy::clear_collision_buffer()
{
	collided_objects.clear();
}

