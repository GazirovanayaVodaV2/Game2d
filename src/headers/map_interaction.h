#pragma once
#include "interactive_object.h"
/// <summary>
/// Only interaction with map, no interactions with entities
/// </summary>
class map_interaction : public interactive_object_base {
protected:
	void interaction_impl(entity* ent) override {};

	OBJECT::TYPE get_type() override { return OBJECT::TYPE::MAP_INTERACTION; }
};