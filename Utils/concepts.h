#pragma once

#include "pch.h"

template <typename T>
concept incrementable = requires(T a) {
    { ++a } -> std::same_as<T&>;
    { a++ } -> std::same_as<T>;
};

template <typename T>
concept decrementable = requires(T a) {
    { --a } -> std::same_as<T&>;
    { a-- } -> std::same_as<T>;
};

template <typename T>
concept addable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { a += b } -> std::same_as<T&>;
};

template <typename T>
concept subtractable = requires(T a, T b) {
    { a - b } -> std::same_as<T>;
    { a -= b } -> std::same_as<T&>;
};

template <typename T>
concept multipliable = requires(T a, T b) {
    { a* b } -> std::same_as<T>;
    { a *= b } -> std::same_as<T&>;
};

template <typename T>
concept divisible = requires(T a, T b) {
    { a / b } -> std::same_as<T>;
    { a /= b } -> std::same_as<T&>;
};

template <typename T>
concept streamable = requires(std::ostream & os, std::istream & is, T a) {
    { os << a } -> std::same_as<std::ostream&>;
    { is >> a } -> std::same_as<std::istream&>;
};

template <typename T>
concept comparable = requires(T a, T b) {
    { a == b } -> std::same_as<bool>;
    { a != b } -> std::same_as<bool>;
    { a < b } -> std::same_as<bool>;
    { a > b } -> std::same_as<bool>;
    { a <= b } -> std::same_as<bool>;
    { a >= b } -> std::same_as<bool>;
};

template <typename T>
concept bitwise_operable = requires(T a, int b) {
    { a >> b } -> std::same_as<T>;
    { a << b } -> std::same_as<T>;
    { a& b } -> std::same_as<T>;
    { a | b } -> std::same_as<T>;
    { ~a } -> std::same_as<T>;
    { a^ b } -> std::same_as<T>;
};

template <typename T>
concept boolean_operable = requires(T a) {
    { !a } -> std::same_as<bool>;
};