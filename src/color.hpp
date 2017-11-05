#ifndef ML_COLOR_HPP
#define ML_COLOR_HPP

#include <cstdint>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <exception>
#include <spdlog/spdlog.h>
#include <json/json_fwd.hpp>

struct color
{
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::uint8_t a;

    color();

    color(
        const std::uint8_t r,
        const std::uint8_t g,
        const std::uint8_t b,
        const std::uint8_t a
    );

    // from rgba hex like ffaabb or ffaabbff
    color(const std::string & hex);
};

void to_json(nlohmann::json& j, const color& p);
void from_json(const nlohmann::json& j, color& p);

#endif

