#ifndef ML_BALL_HPP
#define ML_BALL_HPP

#include <cmath>
#include <iostream>
#include <Box2D/Box2D.h>
#include <memory>
#include "settings.hpp"
#include "util.hpp"
#include "collision_user_data.hpp"
#include "ball_type.hpp"
#include "ball_powerup.hpp"
#include "powerup_type.hpp"
#include "flag.hpp"
#include "ball_flag.hpp"
#include "portal.hpp"
#include "explosion.hpp"
#include "player.hpp"

struct collision_user_data;
struct flag;
struct portal;
struct player;

struct ball
{
    ball_type type;
    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;
    portal* portal_transport_ptr;
    bool is_alive;
    int respawn_counter;
    std::vector<ball_powerup> powerups;
    std::vector<ball_flag> flags;
    explosion rb_ex;
    explosion pop_ex;
    player* player_ptr;

    ball(const ball_type type);

    void set_player_ptr(player* p);
    void set_portal_transport(portal* p);
    void add_to_world(b2World * world);
    void set_position(const b2Vec2 pos);
    void move(const int x, const int y);
    b2Vec2 get_position() const;
    float get_angle() const;
    b2Vec2 get_linear_velocity() const;
    void pop();
    void get_boosted();
    void add_powerup(const powerup_type type);
    bool has_powerup(const powerup_type type);
    void remove_powerup(const powerup_type type);
    void rb_explode();
    bool has_flag(const flag_type type);
    void add_flag(flag* f);
    void reset_flags();
    void score();
};

#endif

