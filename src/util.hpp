#ifndef ML_UTIL_HPP
#define ML_UTIL_HPP

#include <vector>
#include <string>
#include <sstream>
#include <cstdint>
#include "polygon.hpp"


std::vector<std::string> split_on(const std::string & str, const char n);

std::vector<polygon> make_square_poly(
    const std::uint32_t x,
    const std::uint32_t y
);

#endif

