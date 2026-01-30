#include "base.h"

#include "entity.h"
#include "interactive_object.h"

bool interactive_object_base::check_collision(game_object* object)
{
	if (not in_inventory())
	{
		entity_nearby = false;
		collided = global_check_collision(this, object);

		if (collided) {
			collided_objects.push_back(object);

			entity_nearby = (object->get_type() == OBJECT::TYPE::ENTITY) ||
				(is_subtype_of(object->get_type(), OBJECT::TYPE::ENTITY));

			if (entity_nearby)
				interact(static_cast<entity*>(object));
		}

		return collided;
	}
	return false;
}

void interactive_object_base::clear_collision_buffer()
{
	collided_objects.clear();
}

void interactive_object_base::interact(entity* ent)
{
	if (entity_nearby) {
		interaction_impl(ent);

		for (auto& c_interaction : custom_interactions) {
			c_interaction(ent);
		}
	}
}

interactive_object_base& interactive_object_base::add_interaction(std::function<void(entity*)> inter)
{
	custom_interactions.emplace_back(inter);
	return *this;
}

bool interactive_object_base::use(entity* user)
{
	if (exist) {
		auto us_center = user->get_pos() + (user->get_size() / 2);
		auto center = pos + (size / 2);
		auto distance = (us_center - center).len();
		if ((distance < use_dist) or (in_inventory())) {
			return use_impl(user);
		}
	}

	return false;
}

void interactive_object_base::pick_up()
{
	if (is_pickable()) {
		physic = false;
		collideable = false;
		in_inventory_ = true;
	}
}


bool interactive_object_base::in_inventory()
{
	return in_inventory_;
}

