#ifndef ML_BALL_HPP
#define ML_BALL_HPP

#include <cmath>
#include <iostream>
#include <Box2D/Box2D.h>
#include <spdlog/spdlog.h>
#include <memory>
#include "settings.hpp"
#include "util.hpp"
#include "collision_user_data.hpp"
#include "ball_type.hpp"
#include "ball_powerup.hpp"
#include "powerup_type.hpp"
#include "flag.hpp"
#include "flag_type.hpp"
#include "ball_flag.hpp"
#include "portal.hpp"
#include "explosion.hpp"
#include "player.hpp"
#include "gate.hpp"

struct collision_user_data;
struct flag;
struct portal;
struct player;
struct ball_flag;

struct ball
{
    static thread_local std::size_t id_counter;
    std::size_t id;
    ball_type type;
    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;
    portal* portal_transport_ptr;
    bool is_alive;
    int respawn_counter;
    std::vector<ball_powerup> powerups;
    std::vector<ball_flag> flags;
    player* player_ptr;
    std::unordered_set<gate*> in_gate_ptrs;

    ball(const ball_type type);
    ~ball();

    void set_player_ptr(player* p);
    void set_portal_transport(portal* p);
    void add_to_world(b2World * world);
    void set_position(const b2Vec2 pos);
    void move(const int x, const int y);
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

