#ifndef ML_TILE_TYPE_HPP
#define ML_TILE_TYPE_HPP

#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include <json/json_fwd.hpp>

enum class tile_type
{
    background,
    normal,
    speed_red,
    speed_blue,
    speed_yellow,
    endzone_red,
    endzone_blue
};

std::string to_string(const tile_type m);
tile_type tile_type_from_string(const std::string & m);

#endif

