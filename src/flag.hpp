#ifndef ML_FLAG_HPP
#define ML_FLAG_HPP

#include <string>
#include <iostream>

#include "libs/json.hpp"
#include "flag_type.hpp"

struct flag
{
    float x;
    float y;
    flag_type type;

    flag() {}
    flag(
        const float x,
        const float y,
        const flag_type type
    )
    : x(x)
    , y(y)
    , type(type)
    {}
};

void to_json(nlohmann::json& j, const flag& p);
void from_json(const nlohmann::json& j, flag& p);

#endif
