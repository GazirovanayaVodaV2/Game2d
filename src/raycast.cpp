#include "raycast.h"
#include <limits>

float raycast::intersect(ray& ray_, const std::shared_ptr<base_object>& obj)
{
    float tmin = 0.0f;
    float tmax = std::numeric_limits<float>::max();

    float inv_dir = 1.0f / ray_.dir.x();

    for (int i = 0; i < 3; i++) {

    }

    return 0.0f;
}
