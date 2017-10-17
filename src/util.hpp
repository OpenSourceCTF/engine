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

std::vector<std::string> split_on(const std::string & str, const char n);

std::vector<polygon> make_square_poly(
    const std::uint32_t x,
    const std::uint32_t y
);

#endif

