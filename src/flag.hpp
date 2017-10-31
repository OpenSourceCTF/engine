#ifndef ML_FLAG_HPP
#define ML_FLAG_HPP

#include <memory>
#include <string>
#include <iostream>

#include <Box2D/Box2D.h>
#include <spdlog/spdlog.h>

#include "libs/json.hpp"
#include "flag_type.hpp"
#include "collision_user_data.hpp"
#include "settings.hpp"
#include "ball.hpp"

struct collision_user_data;
struct ball;

struct flag
{
    float x;
    float y;
    flag_type type;
    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;
    bool is_alive;

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
    , col_data(nullptr)
    , is_alive(true)
    {}

    void add_to_world(b2World * world);
    void step_on(ball* m);
};

void to_json(nlohmann::json& j, const flag& p);
void from_json(const nlohmann::json& j, flag& p);

#endif
