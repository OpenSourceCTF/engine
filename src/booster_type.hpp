#ifndef ML_BOOSTER_TYPE_HPP
#define ML_BOOSTER_TYPE_HPP

#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include <json/json_fwd.hpp>

enum class booster_type
{
    all, blue, red
};

std::string to_string(const booster_type m);
booster_type booster_type_from_string(const std::string & m);

#endif

