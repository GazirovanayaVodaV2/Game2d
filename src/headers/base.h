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
		WALL,
		INTERACTIVE_OBJECT,
		BACKGROUND_SPRITE,
		MAP_INTERACTION,
		LEVEL_SWITCHER
	};

	namespace MAP {
		enum TYPE {
			air,
			brick,
			player_spawn,
			light,
			background_sprite,
			level_switcher,
		};
	}
};

class base
{
protected:
	static inline const OBJECT::TYPE type = OBJECT::TYPE::NULL_OBJECT;
public:
	base() = default;
	virtual ~base() = default;

	base(const base&) = delete;
	base& operator=(const base&) = delete;

	//base(const base&) = default;
	//base& operator=(const base&) = default;

	virtual OBJECT::TYPE get_type() = 0;

	bool is_null() {
		return get_type() == OBJECT::TYPE::NULL_OBJECT;
	}
};

