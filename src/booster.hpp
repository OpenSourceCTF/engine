#ifndef ML_BOOSTER_HPP
#define ML_BOOSTER_HPP

#include <string>
#include <iostream>
#include <Box2D/Box2D.h>
#include "libs/json.hpp"
#include "settings.hpp"
#include "collision_user_data.hpp"
#include "booster_type.hpp"
#include "ball.hpp"


struct booster
{
    float x;
    float y;
    booster_type type;
    b2Body * body;

    booster(){}
    booster(
        const float x,
        const float y,
        const booster_type type
    )
    : x(x)
    , y(y)
    , type(type)
    , body(nullptr)
    {}

    void add_to_world(b2World* world);
    void step_on(ball* m);
};

void to_json(nlohmann::json& j, const booster& p);
void from_json(const nlohmann::json& j, booster& p);

#endif

