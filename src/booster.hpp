#ifndef ML_BOOSTER_HPP
#define ML_BOOSTER_HPP

#include <string>
#include <iostream>
#include "libs/json.hpp"
#include "booster_type.hpp"


struct booster
{
    float x;
    float y;
    booster_type type;

    booster(){}
    booster(
        const float x,
        const float y,
        const booster_type type
    )
    : x(x)
    , y(y)
    , type(type)
    {}
};

void to_json(nlohmann::json& j, const booster& p);
void from_json(const nlohmann::json& j, booster& p);

#endif

