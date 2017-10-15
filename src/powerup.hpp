#ifndef ML_POWERUP_HPP
#define ML_POWERUP_HPP

#include "libs/json.hpp"

struct powerup
{
    float x;
    float y;

    powerup(){}
    powerup(
        const float x,
        const float y
    )
    : x(x)
    , y(y)
    {}
};

void to_json(nlohmann::json& j, const powerup& p);
void from_json(const nlohmann::json& j, powerup& p);

#endif

