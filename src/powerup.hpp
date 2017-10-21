#ifndef ML_POWERUP_HPP
#define ML_POWERUP_HPP

#include <memory>
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
    std::shared_ptr<collision_user_data> col_data;
    bool is_alive;
    int respawn_counter;

    powerup(){}
    powerup(
        const float x,
        const float y
    )
    : x(x)
    , y(y)
    , body(nullptr)
    , col_data(nullptr)
    , is_alive(true)
    , respawn_counter(0)
    {}

    void add_to_world(b2World * world);
    void step_on(ball* m);
};

void to_json(nlohmann::json& j, const powerup& p);
void from_json(const nlohmann::json& j, powerup& p);

#endif

