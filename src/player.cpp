#include "player.h"
#include "utils.h"

#include "map.h"
#include "projectile.h"

#include "basic_gun.h"

player::player(SDL_Renderer* render, std::string animation_config)
	:invent(this)
{
	print::loading("Spawning player");
	animations = std::make_unique<animation::player>(render, animation_config);

	auto first_frame = animations->get()->get()->get();
	first_frame->get_size(&size.x, & size.y);

	print::loaded("Player spawned");
};
player::~player()
{
	print::info("Despawning player");
}

SDL_AppResult player::update(float delta_time)
{
	invent.update(delta_time);

	statuses.on_floor = false;
	if ((!fly) && (!noclip)) {
		movement_velocity = movement_velocity + vec2(0.0f, fps::synch(0.4f));
	}

	if (!noclip) {
		for (auto& collided_object : collided_objects) {
			if (collided_object->get_collide_mode()) {
				auto object_pos = collided_object->get_pos();
				auto object_size = collided_object->get_size();

				auto object_left_side = ((pos.x + size.x) >= object_pos.x) &&
					((pos.x + size.x) < (object_pos.x + object_size.x));
				auto object_right_side = ((pos.x) <= (object_pos.x + object_size.x)) &&
					((pos.x) > (object_pos.x));

				auto left_and_right_sides_check = ((pos.y + size.y) > (object_pos.y + (object_size.y * 0.25f))) &&
					(pos.y < (object_pos.y + object_size.y - (object_size.y * 0.25f)));

				if (!left_and_right_sides_check) {
					object_left_side = false;
					object_right_side = false;
				}

				auto object_top_side = ((pos.y + size.y) >= object_pos.y) &&
					((pos.y + size.y) < (object_pos.y + object_size.y));
				auto object_bottom_side = ((pos.y) <= (object_pos.y + object_size.y)) &&
					((pos.y) > (object_pos.y));

				if (object_left_side) {
					if (movement_velocity.x > 0.0f) {
						movement_velocity = vec2(0.0f, movement_velocity.y);
					}
				}
				else if (object_right_side) {
					if (movement_velocity.x < 0.0f) {
						movement_velocity = vec2(0.0f, movement_velocity.y);
					}
				}
				if (object_top_side) {
					if (movement_velocity.y > 0.0f) {
						if (action == ENTITY::ACTION::FALLING) {
							action = ENTITY::ACTION::IDLE;
						}
						statuses.on_floor = true;
						movement_velocity = vec2(movement_velocity.x, 0.0f);
					}
				}
				else if (object_bottom_side) {
					if (movement_velocity.y < 0.0f) {
						movement_velocity = vec2(movement_velocity.x, 0.0f);
					}
				}
			}
		}
	}
	clear_collision_buffer();

	statuses.can_jump = statuses.on_floor;

	if (!(action == ENTITY::ACTION::MOVING_BY_KEYBOARD)) {
		if (!(action == ENTITY::ACTION::FALLING)) {
			if (movement_velocity.len() > 0.01f) {
				movement_velocity = movement_velocity / (fps::synch(0.5f) + 1.0f);
			}
			else {
				movement_velocity = vec2();
			}
		}
	}

	if (movement_velocity.y > 0) {
		action = ENTITY::ACTION::FALLING;
	}
	else if (movement_velocity.len() == 0) {
		action = ENTITY::ACTION::IDLE;
	}


	switch (action)
	{
	case ENTITY::ACTION::IDLE: {
		animations->play("idle", convert::delta2fps(delta_time));
	} break;
	case ENTITY::ACTION::MOVING_BY_KEYBOARD: {
		animations->play("move", convert::delta2fps(delta_time));
	} break;
	case ENTITY::ACTION::FALLING: {
		animations->play("fall", convert::delta2fps(delta_time));
	} break;
	default: {

	} break;
	}

	if (pos.y > 3000.0f) {
		set_pos(vec2());
		movement_velocity = vec2();
	}

	move_on(fps::synch(movement_velocity));
	return SDL_APP_CONTINUE;
}

SDL_AppResult player::input(const SDL_Event* event)
{
	if (!invent.input(event)) {
		if (event->type == SDL_EVENT_KEY_DOWN) {
			switch (event->key.key)
			{
			case SDLK_W: {
				if (statuses.can_jump || fly) {
					action = ENTITY::ACTION::JUMPING;
					movement_velocity = vec2(movement_velocity.x, -default_speed * 5);
				}
			} break;
			case SDLK_S: {
				if (fly) {
					action = ENTITY::ACTION::MOVING_BY_KEYBOARD;
					movement_velocity = vec2(movement_velocity.x, default_speed);
				}
			} break;
			case SDLK_D: {
				if (action != ENTITY::ACTION::JUMPING) {
					action = ENTITY::ACTION::MOVING_BY_KEYBOARD;
					movement_velocity = vec2(default_speed, movement_velocity.y);
					this->dir = OBJECT_DIRECTION::RIGHT;
				}
			} break;
			case SDLK_A: {
				if (action != ENTITY::ACTION::JUMPING) {
					action = ENTITY::ACTION::MOVING_BY_KEYBOARD;
					movement_velocity = vec2(-default_speed, movement_velocity.y);
					this->dir = OBJECT_DIRECTION::LEFT;
				}
			} break;
			case SDLK_K: {
				//temp

				//float bullet_speed = 20.0f;
				//bullet_speed = 1.0f;
				//vec2 bullet_vel = (dir == OBJECT_DIRECTION::RIGHT) ? vec2(bullet_speed, 0.0f) : vec2(-bullet_speed, 0.0f);
				//vec2 bullet_spawn_pos = (dir == OBJECT_DIRECTION::RIGHT) ? vec2(80.0f, 0.0f) : vec2(-32.0f, 0.0f);
				//bullet_spawn_pos += this->pos;
				//level_manager::get()->add_bullet(34, bullet_spawn_pos, bullet_speed, bullet_vel);

				auto it = invent.get_item_in_hands();
				if (it) {
					it->use(this);
				}
			} break;
			case SDLK_E: {
				auto lvl = level_manager::get();
				if (lvl) {
					if (auto* obj = dynamic_cast<interactive_object_base*>(lvl->get(pos))) {
						invent.try_add_item(obj);
					}
					else if (auto* obj = dynamic_cast<interactive_object_base*>(lvl->get(vec2(pos.x, pos.y + size.y / 2)))) {
						invent.try_add_item(obj);
					}
				}
			} break;
			case SDLK_R: {
				auto it = invent.get_item_in_hands();
				if (it) {
					if (auto* gun_ptr = dynamic_cast<basic_gun*>(it)) {
						gun_ptr->reload(this);
					}
				}
			} break;
			case SDLK_F3: {
				fly = !fly;
			} break;
			case SDLK_F4: {
				noclip = !noclip;
			} break;
			default: {
			} break;
			}
		}
		else if (event->type == SDL_EVENT_KEY_UP) {
			switch (event->key.key)
			{
			default: action = ENTITY::ACTION::WALKING;
				break;
			}
		}
	}
	return SDL_APP_CONTINUE;
}

void player::set_pos(vec2 pos)
{
	this->pos = pos;
}
void player::move_on(vec2 velocity)
{
	this->pos += velocity;
}

void player::set_size(vec2 size)
{
	this->size = size;
}

void player::rotate(double angle)
{
	this->angle = angle;
}

vec2 player::get_size()
{
	return size;
}
vec2 player::get_pos()
{
	return pos;
}

float player::get_ratio()
{
	return size.x / size.y;
}

void player::draw()
{
	auto flip = SDL_FLIP_NONE;
	if (dir == OBJECT_DIRECTION::RIGHT) {
		flip = SDL_FLIP_HORIZONTAL;
	}


	auto frame = animations->get()->get()->get();
	frame->draw_rotated(camera::get(),camera::get_pos(), pos, size, angle, flip);

	camera::draw_debug_text(object_msg,  window::get_size() / 2);
	object_msg = "";
}
bool player::check_collision(game_object* object)
{
	if (!dynamic_cast<entity*>(object)) {
		if (object->check_collision(this)) {
			collided_objects.push_back(object);
		}
	}

	collided = collided_objects.size() > 0;

	return collided;
}

void player::clear_collision_buffer()
{
	collided_objects.clear();
}

void player::death()
{
	entity::death();
	set_pos(vec2());
	movement_velocity = vec2();
}

void player::set_hp(int hp)
{
	entity::set_hp(hp);
}

void player::damage(int damage)
{
	entity::damage(damage);
}

void player::heal(int hp)
{
	entity::heal(hp);
}

int player::get_hp()
{
	return hp;
}

void player::get_msg_from_int_obj(std::string msg)
{
	this->object_msg = msg;
}

inventory::inventory& player::get_inventory()
{
	return invent;
}
