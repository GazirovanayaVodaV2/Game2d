#pragma once
#include <string>
#include <ostream>
#include <cmath>

#include <concepts>
#include <functional>
#include <vector>
#include <memory>
#include <map>

#include "Utils/custom_warning.h"

#define cmp_type(object, type) (typeid(object) == typeid(type))

class base
{
public:
	base() = default;
	virtual ~base() = default;

	base(const base&) = delete;
	base& operator=(const base&) = delete;
};