#ifndef ML_GATE_TYPE_HPP
#define ML_GATE_TYPE_HPP

#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include <json/json_fwd.hpp>

enum class gate_type
{
    off, green, blue, red
};

std::string to_string(const gate_type m);
gate_type gate_type_from_string(const std::string & m);

#endif

