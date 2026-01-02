#pragma once

#include "pch.h"

#include "concepts.h"

template <typename value_t>
struct with_default_value {
private:
    value_t value{};
    value_t default_value{};

public:
    with_default_value() = default;
    with_default_value(value_t def) : value(def), default_value(def) {}

    with_default_value(const with_default_value&) = default;
    with_default_value(with_default_value&&) = default;
    with_default_value& operator=(const with_default_value&) = default;
    with_default_value& operator=(with_default_value&&) = default;

    inline void set_new_defaults(value_t new_def) {
        default_value = std::move(new_def);
    }

    inline value_t get_default() {
        return default_value;
    }

    inline value_t& operator*() {
        return value;
    }

    with_default_value& operator=(value_t val) {
        value = val;
        return *this;
    }

    with_default_value(const char* s) {
        static_assert(std::is_same_v<value_t, std::string>,
            "Only for std::string!!!");
        value = std::string(s);
        default_value = value;
    }

    with_default_value& operator=(const char* s) {
        static_assert(std::is_same_v<value_t, std::string>,
            "Only for std::string!!!");
        value = std::string(s);
        return *this;
    }

    with_default_value& operator+=(const with_default_value& other) {
        static_assert(addable<value_t>,
            "Only for types with operator+ support!");
        value += other.value;
        return *this;
    }
    with_default_value& operator-=(const with_default_value& other) {
        static_assert(subtractable<value_t>,
            "Only for types with operator- support!");
        value -= other.value;
        return *this;
    }
    with_default_value& operator*=(const with_default_value& other) {
        static_assert(multipliable<value_t>,
            "Only for types with operator* support!");
        value *= other.value;
        return *this;
    }
    with_default_value& operator/=(const with_default_value& other) {
        static_assert(divisible<value_t>,
            "Only for types with operator/ support!");
        value /= other.value;
        return *this;
    }

    with_default_value& operator++() {
        static_assert(incrementable<value_t>,
            "Only for types with operator++ support!");
        ++value;
        return *this;
    }
    with_default_value operator++(int)
    {
        static_assert(incrementable<value_t>,
            "Only for types with operator++ support!");
        auto tmp = *this;
        ++value;
        return tmp;
    }
    with_default_value& operator--() {
        static_assert(decrementable<value_t>,
            "Only for types with operator-- support!");
        --value;
        return *this;
    }
    with_default_value operator--(int) {
        static_assert(decrementable<value_t>,
            "Only for types with operator-- support!");
        auto tmp = *this;
        --value;
        return tmp;
    }


    operator value_t() const { return value; }


    void reset() { value = default_value; }

    friend std::ostream& operator<<(std::ostream& os, const with_default_value& obj) {
        static_assert(streamable<value_t>,
            "Only for types with stream support!");
        return os << obj.value;
    }
    friend std::istream& operator>>(std::istream& is, with_default_value& obj) {
        static_assert(streamable<value_t>,
            "Only for types with stream support!");
        return is >> obj.value;
    }

    friend bool operator==(const with_default_value& lhs, const value_t& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value == rhs;
    }
    friend bool operator==(const value_t& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs == rhs.value;
    }

    friend bool operator!=(const with_default_value& lhs, const value_t& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value != rhs;
    }
    friend bool operator!=(const value_t& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs != rhs.value;
    }

    friend bool operator<(const with_default_value& lhs, const value_t& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value < rhs;
    }
    friend bool operator<(const value_t& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs < rhs.value;
    }

    friend bool operator>(const with_default_value& lhs, const value_t& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value > rhs;
    }
    friend bool operator>(const value_t& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs > rhs.value;
    }

    friend bool operator<=(const with_default_value& lhs, const value_t& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value <= rhs;
    }
    friend bool operator<=(const value_t& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs <= rhs.value;
    }

    friend bool operator>=(const with_default_value& lhs, const value_t& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value >= rhs;
    }
    friend bool operator>=(const value_t& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs >= rhs.value;
    }

    friend bool operator==(const with_default_value& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value == rhs.value;
    }
    friend bool operator!=(const with_default_value& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value != rhs.value;
    }
    friend bool operator<(const with_default_value& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value < rhs.value;
    }
    friend bool operator>(const with_default_value& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value > rhs.value;
    }
    friend bool operator<=(const with_default_value& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value <= rhs.value;
    }
    friend bool operator>=(const with_default_value& lhs, const with_default_value& rhs) {
        static_assert(comparable<value_t>,
            "Only for comparable types!");
        return lhs.value >= rhs.value;
    }

    friend with_default_value operator+(with_default_value lhs, const with_default_value& rhs) {
        static_assert(addable<value_t>,
            "Only for types with operator+ support!");
        lhs += rhs;
        return lhs;
    }
    friend with_default_value operator-(with_default_value lhs, const with_default_value& rhs) {
        static_assert(subtractable<value_t>,
            "Only for types with operator- support!");
        lhs -= rhs;
        return lhs;
    }
    friend with_default_value operator*(with_default_value lhs, const with_default_value& rhs) {
        static_assert(multipliable<value_t>,
            "Only for types with operator* support!");
        lhs *= rhs;
        return lhs;
    }
    friend with_default_value operator/(with_default_value lhs, const with_default_value& rhs) {
        static_assert(divisible<value_t>,
            "Only for types with operator/ support!");
        lhs /= rhs;
        return lhs;
    }

    friend with_default_value operator>>(with_default_value lhs, int rhs) {
        static_assert(bitwise_operable<value_t>,
            "operator>> not allowed for this type!");
        lhs.value >>= rhs;
        return lhs;
    }

    friend with_default_value operator<<(with_default_value lhs, int rhs) {
        static_assert(bitwise_operable<value_t>,
            "operator<< not allowed for this type!");

        lhs.value <<= rhs;
        return lhs;
    }

    friend with_default_value operator&(with_default_value lhs, int rhs) {
        static_assert(bitwise_operable<value_t>,
            "operator& not allowed for this type!");

        lhs.value &= rhs;
        return lhs;
    }
    friend with_default_value operator|(with_default_value lhs, int rhs) {
        static_assert(bitwise_operable<value_t>,
            "operator| not allowed for this type!");

        lhs.value |= rhs;
        return lhs;
    }
    friend with_default_value operator~(with_default_value lhs) {
        static_assert(bitwise_operable<value_t>,
            "operator~ not allowed for this type!");

        return with_default_value(~lhs.value);
    }
    friend with_default_value operator^(with_default_value lhs, int rhs) {
        static_assert(bitwise_operable<value_t>,
            "operator<< not allowed for this type!");

        lhs.value <<= rhs;
        return lhs;
    }

    friend bool operator!(const with_default_value& lhs) {
        static_assert(boolean_operable<value_t>,
            "operator! not allowed for this type!");
        return !lhs.value;
    }
};

namespace std {
    template <typename T>
    T abs(const with_default_value<T>& x) {
        static_assert(std::is_arithmetic_v<T>, "abs support only arithmetic types!");
        return std::abs(static_cast<T>(x));
    }

    template <typename T>
    T sqrt(const with_default_value<T>& x) {
        static_assert(std::is_arithmetic_v<T>, "sqrt support only arithmetic types!");
        return std::sqrt(static_cast<T>(x));
    }

    template <typename T>
    T sin(const with_default_value<T>& x) {
        static_assert(std::is_arithmetic_v<T>, "sin support only arithmetic types!");
        return std::sin(static_cast<T>(x));
    }

    template <typename T>
    T cos(const with_default_value<T>& x) {
        static_assert(std::is_arithmetic_v<T>, "cos support only arithmetic types!");
        return std::cos(static_cast<T>(x));
    }

    template <typename T>
    T tan(const with_default_value<T>& x) {
        static_assert(std::is_arithmetic_v<T>, "tan support only arithmetic types!");
        return std::tan(static_cast<T>(x));
    }

    template <typename T>
    T exp(const with_default_value<T>& x) {
        static_assert(std::is_arithmetic_v<T>, "exp support only arithmetic types!");
        return std::exp(static_cast<T>(x));
    }

    template <typename T>
    T log(const with_default_value<T>& x) {
        static_assert(std::is_arithmetic_v<T>, "log support only arithmetic types!");
        return std::log(static_cast<T>(x));
    }

    template <typename T>
    T pow(const with_default_value<T>& x, const with_default_value<T>& y) {
        static_assert(std::is_arithmetic_v<T>, "pow support only arithmetic types!");
        return std::pow(static_cast<T>(x), static_cast<T>(y));
    }

    template <typename T>
    T floor(const with_default_value<T>& x) {
        static_assert(std::is_arithmetic_v<T>, "floor support only arithmetic types!");
        return std::floor(static_cast<T>(x));
    }

    template <typename T>
    T ceil(const with_default_value<T>& x) {
        static_assert(std::is_arithmetic_v<T>, "ceil support only arithmetic types!");
        return std::ceil(static_cast<T>(x));
    }

    template <typename T>
    T round(const with_default_value<T>& x) {
        static_assert(std::is_arithmetic_v<T>, "round support only arithmetic types!");
        return std::round(static_cast<T>(x));
    }

    template <typename T>
    T fmod(const with_default_value<T>& x, const with_default_value<T>& y) {
        static_assert(std::is_arithmetic_v<T>, "fmod support only arithmetic types!");
        return std::fmod(static_cast<T>(x), static_cast<T>(y));
    }

};

