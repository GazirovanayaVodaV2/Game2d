#pragma once
#include <memory>
#include "vector.h"
#include "game_object.h"
#include "Utils/custom_warning.h"

COMPILER_WARNING("Raycast is still WIP, this code cause bugs!");
namespace raycast {
	struct ray {
		vec2 origin, dir;
		float angle = 0.0f;
	};

	bool intersect(ray& ray_, const std::shared_ptr<game_object>& obj, float& dist);
	const std::shared_ptr<game_object>& cast(ray& ray_,
		const std::vector<std::shared_ptr<game_object>>& objs, float& dist);
}

