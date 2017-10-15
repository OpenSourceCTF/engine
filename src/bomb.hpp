#ifndef ML_BOMB_HPP
#define ML_BOMB_HPP

#include <iostream>
#include <Box2D/Box2D.h>
#include "libs/json.hpp"
#include "settings.hpp"
#include "collision_user_data.hpp"

struct bomb
{
    float x;
    float y;
    b2Body * body;

    bomb(){}
    bomb(
        const float x,
        const float y
    )
    : x(x)
    , y(y)
    , body(nullptr)
    {}

    void add_to_world(b2World* world);
    void explode();
};

void to_json(nlohmann::json& j, const bomb& p);
void from_json(const nlohmann::json& j, bomb& p);

#endif

