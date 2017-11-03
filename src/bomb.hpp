#ifndef ML_BOMB_HPP
#define ML_BOMB_HPP

#include <memory>
#include <iostream>
#include <cmath>
#include <Box2D/Box2D.h>
#include <json.hpp>
#include "settings.hpp"
#include "collision_user_data.hpp"
#include "ball.hpp"
#include "util.hpp"
#include "explosion.hpp"

struct ball;
struct collision_user_data;

struct bomb
{
    static thread_local std::size_t id_counter;
    std::size_t id;
    float x;
    float y;
    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;
    bool is_alive;
    int respawn_counter;

    bomb() {}
    bomb(
        const float x,
        const float y
    );
    ~bomb();

    void add_to_world(b2World* world);
    void explode();
};

void to_json(nlohmann::json& j, const bomb& p);
void from_json(const nlohmann::json& j, bomb& p);

#endif

