#ifndef ML_COLLISION_USER_DATA_HPP
#define ML_COLLISION_USER_DATA_HPP

#include "collision_user_data_type.hpp"

#include "ball.hpp"
#include "wall.hpp"
#include "spike.hpp"
#include "bomb.hpp"
#include "toggle.hpp"
#include "booster.hpp"
#include "powerup.hpp"
#include "flag.hpp"
#include "portal.hpp"

// forward declares for constructors
struct ball;
struct wall;
struct spike;
struct bomb;
struct toggle;
struct booster;
struct powerup;
struct flag;
struct portal;

struct collision_user_data
{
    void * ptr;
    collision_user_data_type type;

    collision_user_data(): ptr(nullptr), type(collision_user_data_type::unknown)  {}
    collision_user_data(ball* ptr):    ptr(ptr), type(collision_user_data_type::ball){}
    collision_user_data(wall* ptr):    ptr(ptr), type(collision_user_data_type::wall){}
    collision_user_data(spike* ptr):   ptr(ptr), type(collision_user_data_type::spike){}
    collision_user_data(bomb* ptr):    ptr(ptr), type(collision_user_data_type::bomb){}
    collision_user_data(toggle* ptr):  ptr(ptr), type(collision_user_data_type::toggle){}
    collision_user_data(booster* ptr): ptr(ptr), type(collision_user_data_type::booster){}
    collision_user_data(powerup* ptr): ptr(ptr), type(collision_user_data_type::powerup){}
    collision_user_data(flag* ptr):    ptr(ptr), type(collision_user_data_type::flag){}
    collision_user_data(portal* ptr):  ptr(ptr), type(collision_user_data_type::portal){}
};

#endif

