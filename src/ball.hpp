#ifndef ML_BALL_HPP
#define ML_BALL_HPP

#include <cmath>
#include <iostream>
#include <Box2D/Box2D.h>
#include "settings.hpp"
#include "collision_user_data.hpp"
#include "ball_type.hpp"
#include "util.hpp"

struct ball
{
    ball_type type;
    bool is_alive;
    std::string name;
    bool is_registered;
    std::string user_id;
    std::uint32_t degree;
    b2Body * body;

    ball(const ball_type type)
    : type(type)
    , is_alive(true)
    , name("someball")
    , is_registered(false)
    , user_id("000000-0000-0000-000000")
    , degree(0)
    , body(nullptr)
    {}

    void add_to_world(b2World * world);
    void set_position(const b2Vec2 pos);
    void move(const int x, const int y);
    b2Vec2 get_position() const;
    float get_angle() const;
    b2Vec2 get_linear_velocity() const;
    void pop();
    void get_boosted();
};

#endif

