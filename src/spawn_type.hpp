#ifndef ML_SPAWN_TYPE_HPP
#define ML_SPAWN_TYPE_HPP

#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include <json/json_fwd.hpp>

enum class spawn_type
{
    blue, red
};

std::string to_string(const spawn_type m);
spawn_type spawn_type_from_string(const std::string & m);

#endif

