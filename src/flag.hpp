#ifndef ML_FLAG_HPP
#define ML_FLAG_HPP

#include <string>
#include <iostream>

#include <Box2D/Box2D.h>

#include "libs/json.hpp"
#include "flag_type.hpp"
#include "collision_user_data.hpp"
#include "settings.hpp"
#include "ball.hpp"

struct flag
{
    float x;
    float y;
    flag_type type;
    b2Body * body;
    collision_user_data* col_data;

    flag() {}
    flag(
        const float x,
        const float y,
        const flag_type type
    )
    : x(x)
    , y(y)
    , type(type)
    , body(nullptr)
    , col_data(new collision_user_data(this))
    {}

    void add_to_world(b2World * world);
    void step_on(ball* m);
};

void to_json(nlohmann::json& j, const flag& p);
void from_json(const nlohmann::json& j, flag& p);

#endif
