#ifndef ML_TOGGLE_TAG_TYPE_HPP
#define ML_TOGGLE_TAG_TYPE_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include "libs/json.hpp"

enum class toggle_tag_type
{
    bomb, gate
};

std::string to_string(const toggle_tag_type m);
toggle_tag_type toggle_tag_type_from_string(const std::string & m);

#endif

