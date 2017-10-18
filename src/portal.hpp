#ifndef ML_PORTAL_HPP
#define ML_PORTAL_HPP

#include <memory>
#include <cstdint>
#include <Box2D/Box2D.h>
#include "libs/json.hpp"

#include "collision_user_data.hpp"
#include "settings.hpp"
#include "ball.hpp"

struct portal
{
    float x;
    float y;

    bool has_cooldown;
    std::uint32_t cooldown;

    bool has_destination;
    float dx;
    float dy;

    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;

    portal() {}
    portal(
        const float x,
        const float y
    )
    : x(x)
    , y(y)
    , has_cooldown(false)
    , has_destination(false)
    , body(nullptr)
    , col_data(nullptr)
    {}

    void set_cooldown(const std::uint32_t x);
    void set_destination(const float x, const float y);

    void add_to_world(b2World * world);
    void step_on(ball* m);
};

void to_json(nlohmann::json& j, const portal& p);
void from_json(const nlohmann::json& j, portal& p);

#endif

