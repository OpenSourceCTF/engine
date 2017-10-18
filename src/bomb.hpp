#ifndef ML_BOMB_HPP
#define ML_BOMB_HPP

#include <memory>
#include <iostream>
#include <cmath>
#include <Box2D/Box2D.h>
#include "libs/json.hpp"
#include "settings.hpp"
#include "collision_user_data.hpp"
#include "ball.hpp"
#include "util.hpp"

struct bomb
{
    float x;
    float y;
    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;

    bomb(){}
    bomb(
        const float x,
        const float y
    )
    : x(x)
    , y(y)
    , body(nullptr)
    , col_data(nullptr)
    {}

    void add_to_world(b2World* world);
    void explode(ball* b);
};

void to_json(nlohmann::json& j, const bomb& p);
void from_json(const nlohmann::json& j, bomb& p);

#endif

