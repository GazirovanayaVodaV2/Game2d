#pragma once
#include "interactive_object.h"
/// <summary>
/// Only interaction with map, no interactions with entities
/// </summary>
class map_interaction : public interactive_object_base {
protected:
	static inline const OBJECT::TYPE type = OBJECT::TYPE::MAP_INTERACTION;

	void interaction_impl(std::shared_ptr<entity> ent) override {};
};