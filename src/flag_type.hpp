#ifndef ML_FLAG_TYPE_HPP
#define ML_FLAG_TYPE_HPP

#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include <json/json_fwd.hpp>

enum class flag_type
{
    neutral, blue, red
};

std::string to_string(const flag_type m);
flag_type flag_type_from_string(const std::string & m);

#endif

