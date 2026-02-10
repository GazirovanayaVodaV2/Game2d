#include "raycast.h"
#include <limits>
#include <cmath>
#include <cstdlib>


//WIP

bool raycast::intersect(ray& ray_, const std::shared_ptr<game_object>& obj, float& dist)
{
    float tmin = 0.0f;
    float tmax = std::numeric_limits<float>::max();

    float inv_dir = 1.0f / ray_.dir.x;

    vec2 axes[2] = {
        vec2(cosf(ray_.angle), sinf(ray_.angle)),
        vec2(-sinf(ray_.angle), cosf(ray_.angle))
    };

    float halfsize[2] = { obj->get_size().x / 2, obj->get_size().y / 2 };
    auto obj_center = obj->get_pos() + (obj->get_size() / 2);
    auto delta = obj_center - ray_.origin;

    for (int i = 0; i < 2; i++) {
        float e = delta.x * axes[i].x + delta.y * axes[i].y;
        float f = ray_.dir.x * axes[i].x + ray_.dir.y * axes[i].y;

        if (abs(f) > 0.000001f) {
            float t1 = (e + halfsize[i]) / f;
            float t2 = (e - halfsize[i]) / f;
            if (t1 > t2) std::swap(t1, t2);

            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;

            if (tmin > tmax || tmax < 0) return false;
        }
    }
    dist = tmin;
    return true;
}

const std::shared_ptr<game_object>& raycast::cast(ray& ray_,
    const std::vector<std::shared_ptr<game_object>>& objs, float& dist)
{
    size_t min_id = 0, id = 0;
    float min_val = std::numeric_limits<float>::max();

    for (auto& obj : objs) {
        if (intersect(ray_, obj, dist)) {
            if (dist < min_val) {
                min_val = dist;
                min_id = id;
            }
        }
        id++;
    }

    return objs[min_id];
}
