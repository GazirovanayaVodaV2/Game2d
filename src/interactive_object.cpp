#include "base.h"

#include "entity.h"
#include "interactive_object.h"

bool interactive_object_base::check_collision(std::shared_ptr<game_object> object)
{
	auto this_rect = pos.get_frect(size);
	auto object_rect = object->get_pos().get_frect(object->get_size());

	bool vertical_collision = ((this_rect.y + this_rect.h) > object_rect.y) &&
		(this_rect.y < (object_rect.y + object_rect.h));

	bool horizontal_collision = ((this_rect.x + this_rect.w) > object_rect.x) &&
		(this_rect.x < (object_rect.x + object_rect.w));
	collided = (vertical_collision && horizontal_collision);

	if (collided) {
		collided_objects.push_back(object);

		entity_nearby = (object->get_type() == OBJECT::TYPE::ENTITY);
		
		if (entity_nearby) 
			interact(std::static_pointer_cast<entity>(object));
	}

	return collided;
}

void interactive_object_base::clear_collision_buffer()
{
	collided_objects.clear();
}

void interactive_object_base::interact(std::shared_ptr<entity> ent)
{
	if (entity_nearby) {
		interaction_impl(ent);

		for (auto& c_interaction : custom_interactions) {
			c_interaction(ent);
		}
	}
}

interactive_object_base& interactive_object_base::add_interaction(std::function<void(std::shared_ptr<entity>)> inter)
{
	custom_interactions.emplace_back(inter);
	return *this;
}

void interactive_object_base::use(std::shared_ptr<entity> user)
{
	auto us_center = user->get_pos() + (user->get_size() / 2);
	auto center = pos + (size / 2);
	auto distance = (us_center - center).len();
	if (distance < use_dist) {
		use_impl(user);
	}
}
