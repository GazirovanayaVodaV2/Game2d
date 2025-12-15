#pragma once
#include <memory>
#include "vector.h"
#include "base_object.h"

namespace raycast {
	struct ray {
		vec2 origin, dir;
	};

	float intersect(ray& ray_, const std::shared_ptr<base_object>& obj);
	std::shared_ptr<base_object>& cast(ray& ray_, const std::vector<std::shared_ptr<base_object>>& objs);
}

