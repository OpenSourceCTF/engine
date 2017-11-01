#ifndef ML_POWERUP_TYPE_HPP
#define ML_POWERUP_TYPE_HPP

#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include <json.hpp>

enum class powerup_type
{
    jukejuice, rollingbomb, tagpro
};

std::string to_string(const powerup_type m);
powerup_type powerup_type_from_string(const std::string & m);

#endif

