#ifndef ML_PORTAL_HPP
#define ML_PORTAL_HPP

#include <memory>
#include <cstdint>
#include <Box2D/Box2D.h>
#include <json/json.hpp>

#include "game_accessor.hpp"
#include "collision_user_data.hpp"
#include "settings.hpp"
#include "ball.hpp"

struct collision_user_data;
struct ball;

struct portal
{
    static thread_local std::size_t id_counter;
    game_accessor game;
    std::size_t id;
    float x;
    float y;

    bool has_cooldown;
    std::uint32_t cooldown;
    bool is_cooling_down;
    std::uint32_t cooldown_counter;

    bool has_destination;
    std::size_t destination_id;
    portal* destination_ptr;

    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;
    bool is_alive;


    portal();
    portal(
        const float x,
        const float y
    );
    ~portal();

    void set_cooldown(const std::uint32_t x);
    void set_destination(const std::size_t id);
    void add_to_world(b2World * world);
    void step_on(ball* m);
    void step_off(ball* m);
};

void to_json(nlohmann::json& j, const portal& p);
void from_json(const nlohmann::json& j, portal& p);

#endif

