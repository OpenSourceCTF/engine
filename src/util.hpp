#ifndef ML_UTIL_HPP
#define ML_UTIL_HPP

#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <random>
#include <type_traits>
#include <exception>
#include <iostream>
#include <memory>
#include "polygon.hpp"

constexpr float PI     = 3.141592653589793;
constexpr float TWO_PI = 6.283185307179586;

float angle_from_input(const int x, const int y);

// v from ax->ay to bx->by
float map_val(
    const float v,
    const float ax,
    const float ay,
    const float bx,
    const float by
);

float dist(
    const float x1,
    const float y1,
    const float x2,
    const float y2
);

std::vector<std::string> split_on(const std::string & str, const char n);

// check if two enum classes have same color
template <typename T, typename U>
constexpr bool same_color(const T a, const U b)
{
    static_assert(std::is_enum<T>::value, "T must be enum");
    static_assert(std::is_enum<U>::value, "U must be enum");

    return (a == T::red  && b == U::red)
        || (a == T::blue && b == U::blue);
}

// return the corresponding color from U a to T type
template <typename T, typename U>
T corresponding_color(const U a)
{
    static_assert(std::is_enum<T>::value, "T must be enum");
    static_assert(std::is_enum<U>::value, "U must be enum");

    switch(a) {
        case U::red:  return T::red;
        case U::blue: return T::blue;
        default:
            std::cerr << "error: corresponding_color doesn't exist" << std::endl;
            std::terminate();
            return T::red; // error suppressor
    }
}

template <typename T>
std::vector<T> uniq_ptr_vec_to_vec(
    const std::vector<std::unique_ptr<T>> & v
) {
    std::vector<T> ret;
    ret.resize(v.size());

    for(auto & o : v) {
        ret.emplace_back(*o);
    }

    return ret;
}


template <typename T>
std::vector<std::unique_ptr<T>> vec_to_uniq_ptr_vec(
    const std::vector<T> & v
) {
    std::vector<std::unique_ptr<T>> ret;
    ret.reserve(v.size());

    for(auto & o : v) {
        ret.emplace_back(new T(o));
    }

    return ret;
}

#endif

