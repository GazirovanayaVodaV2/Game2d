#pragma once
#include <string>
#include <ostream>
#include <cmath>

#include <concepts>
#include <functional>
#include <vector>
#include <memory>
#include <map>

namespace OBJECT {
	enum class TYPE: unsigned int {
		NULL_OBJECT,
		BASE_OBJECT,
		GAME_OBJECT,
		WINDOW,
		CAMERA,
		GAME,
		MAP,
		TEXTURE,
		ATLAS,
		ANIMATION_FRAME,
		ANIMATION_CYCLE,
		ANIMATION_PLAYER,
		LIGHT_SOURCE,
		LIGHT_SYSTEM,
		ENTITY,
		PLAYER,
		DUMMY_ENTITY,
		WALL,
		INTERACTIVE_OBJECT,
		BACKGROUND_SPRITE,
		MAP_INTERACTION,
		LEVEL_SWITCHER,
		PROJECTILE,
	};

	namespace MAP {
		enum TYPE {
			air,
			brick,
			player_spawn,
			light,
			background_sprite,
			level_switcher,
			dummy_entity
		};
	}
};

inline bool is_subtype_of(OBJECT::TYPE type, OBJECT::TYPE base_type) {
    if (type == base_type) return true;
    
    switch (base_type) {
        case OBJECT::TYPE::ENTITY:
            return type == OBJECT::TYPE::PLAYER || 
                   type == OBJECT::TYPE::DUMMY_ENTITY;
        case OBJECT::TYPE::INTERACTIVE_OBJECT:
            return type == OBJECT::TYPE::PROJECTILE;
        default:
            return false;
    }
}

class base
{
public:
	base() = default;
	virtual ~base() = default;

	base(const base&) = delete;
	base& operator=(const base&) = delete;

	//base(const base&) = default;
	//base& operator=(const base&) = default;

	virtual OBJECT::TYPE get_type() {
		return OBJECT::TYPE::NULL_OBJECT;
	};

	bool is_null() {
		return get_type() == OBJECT::TYPE::NULL_OBJECT;
	}
};

