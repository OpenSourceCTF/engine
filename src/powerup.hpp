#ifndef ML_POWERUP_HPP
#define ML_POWERUP_HPP

#include <Box2D/Box2D.h>
#include "libs/json.hpp"
#include "collision_user_data.hpp"
#include "settings.hpp"
#include "ball.hpp"

struct powerup
{
    float x;
    float y;
    b2Body * body;
    collision_user_data* col_data;

    powerup(){}
    powerup(
        const float x,
        const float y
    )
    : x(x)
    , y(y)
    , col_data(new collision_user_data(this))
    {}

    void add_to_world(b2World * world);
    void step_on(ball* m);
};

void to_json(nlohmann::json& j, const powerup& p);
void from_json(const nlohmann::json& j, powerup& p);

#endif

