#ifndef ML_SPIKE_HPP
#define ML_SPIKE_HPP

#include <Box2D/Box2D.h>
#include "libs/json.hpp"
#include "collision_user_data.hpp"
#include "settings.hpp"

struct spike
{
    float x;
    float y;
    b2Body * body;
    collision_user_data* col_data;

    spike(){}
    spike(
        const float x,
        const float y
    )
    : x(x)
    , y(y)
    , body(nullptr)
    , col_data(new collision_user_data(this))
    {}

    void add_to_world(b2World * world);
};

void to_json(nlohmann::json& j, const spike& p);
void from_json(const nlohmann::json& j, spike& p);


#endif

