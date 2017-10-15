#ifndef ML_BOMB_HPP
#define ML_BOMB_HPP

#include <iostream>
#include "libs/json.hpp"

struct bomb
{
    float x;
    float y;

    bomb(){}
    bomb(
        const float x,
        const float y
    )
    : x(x)
    , y(y)
    {}

    void explode();
};

void to_json(nlohmann::json& j, const bomb& p);
void from_json(const nlohmann::json& j, bomb& p);

#endif

