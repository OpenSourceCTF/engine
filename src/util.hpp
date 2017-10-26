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
#include <map>
#include <unordered_set>
#include "polygon.hpp"
#include "chain.hpp"

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
// todo: this has a terrible name
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
std::vector<std::unique_ptr<T>> vec_to_uniq_ptr_vec(
    const std::vector<T> & v
) {
    std::vector<std::unique_ptr<T>> ret;
    ret.reserve(v.size());

    for(auto && o : v) {
        ret.emplace_back(new T(o));
    }

    return ret;
}

template <typename T>
std::vector<std::shared_ptr<T>> vec_to_shrd_ptr_vec(
    const std::vector<T> & v
) {
    std::vector<std::shared_ptr<T>> ret;
    ret.reserve(v.size());

    for(auto && o : v) {
        ret.emplace_back(new T(o));
    }

    return ret;
}

std::vector<std::unique_ptr<chain>> poly2chain(std::vector<polygon> poly_set);

//color codes for each chain so we can inspect which is which
//http://godsnotwheregodsnot.blogspot.com.es/2012/09/color-distribution-methodology.html
constexpr int DISTINCT_COLORS[63][3] =
{
    {1, 0, 103},
    {213, 255, 0},
    {255, 0, 86},
    {158, 0, 142},
    {14, 76, 161},
    {255, 229, 2},
    {0, 95, 57},
    {0, 255, 0},
    {149, 0, 58},
    {255, 147, 126},
    {164, 36, 0},
    {0, 21, 68},
    {145, 208, 203},
    {98, 14, 0},
    {107, 104, 130},
    {0, 0, 255},
    {0, 125, 181},
    {106, 130, 108},
    {0, 174, 126},
    {194, 140, 159},
    {190, 153, 112},
    {0, 143, 156},
    {95, 173, 78},
    {255, 0, 0},
    {255, 0, 246},
    {255, 2, 157},
    {104, 61, 59},
    {255, 116, 163},
    {150, 138, 232},
    {152, 255, 82},
    {167, 87, 64},
    {1, 255, 254},
    {255, 238, 232},
    {254, 137, 0},
    {189, 198, 255},
    {1, 208, 255},
    {187, 136, 0},
    {117, 68, 177},
    {165, 255, 210},
    {255, 166, 254},
    {119, 77, 0},
    {122, 71, 130},
    {38, 52, 0},
    {0, 71, 84},
    {67, 0, 44},
    {181, 0, 255},
    {255, 177, 103},
    {255, 219, 102},
    {144, 251, 146},
    {126, 45, 210},
    {189, 211, 147},
    {229, 111, 254},
    {222, 255, 116},
    {0, 255, 120},
    {0, 155, 255},
    {0, 100, 1},
    {0, 118, 255},
    {133, 169, 0},
    {0, 185, 23},
    {120, 130, 49},
    {0, 255, 198},
    {255, 110, 65},
    {232, 94, 190}
};

#endif
