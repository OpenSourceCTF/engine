#ifndef ML_BALL_HPP
#define ML_BALL_HPP

#include <Box2D/Box2D.h>
#include "settings.hpp"

enum class ball_type
{
    red, blue
};

struct ball
{
    ball_type type;
    b2Body * body;
    std::string name;
    bool is_registered;
    std::string user_id;
    std::uint32_t degree;

    ball(const ball_type type)
    : type(type)
    , body(nullptr)
    {}

    void add_to_world(b2World * world);
    void move(int x, int y);
    b2Vec2 get_position();
    float get_angle();
    b2Vec2 get_linear_velocity();
};

#endif

