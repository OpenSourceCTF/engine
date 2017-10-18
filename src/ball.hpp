#ifndef ML_BALL_HPP
#define ML_BALL_HPP

#include <cmath>
#include <iostream>
#include <Box2D/Box2D.h>
#include <memory>
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
    std::shared_ptr<collision_user_data> col_data;
    bool should_transport;
    std::size_t portal_transport_id;

    ball(const ball_type type)
    : type(type)
    , is_alive(true)
    , name("someball")
    , is_registered(false)
    , user_id("000000-0000-0000-000000")
    , degree(0)
    , body(nullptr)
    , col_data(nullptr)
    , should_transport(false)
    {}

    void set_portal_transport(const std::size_t portal_id);
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

