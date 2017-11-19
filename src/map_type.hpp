#ifndef ML_MAP_TYPE_HPP
#define ML_MAP_TYPE_HPP

#include <iostream>
#include <string>
#include <spdlog/spdlog.h>
#include <json/json_fwd.hpp>

enum class map_type
{
    normal, none_gravityCTF, gravity, gravityCTF
};

std::string to_string(const map_type m);
map_type map_type_from_string(const std::string & m);

#endif
