#pragma once

#include <typeinfo>

template<typename T> struct is_type_info : std::false_type {};
template<> struct is_type_info<std::type_info> : std::true_type {};

#define type_cmp(object, arg) ([&](){ \
    if constexpr (std::is_same_v<decltype(arg), std::type_info>) \
        return typeid(*(object)) == (arg); \
    else \
        return typeid(*(object)) == typeid(arg); \
}())