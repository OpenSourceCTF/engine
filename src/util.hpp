#ifndef ML_UTIL_HPP
#define ML_UTIL_HPP

#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include <random>
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

std::vector<polygon> make_square_poly(
    const std::uint32_t x,
    const std::uint32_t y
);

#endif

