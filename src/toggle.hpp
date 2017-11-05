#ifndef ML_TOGGLE_HPP
#define ML_TOGGLE_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <memory>
#include <json/json_fwd.hpp>
#include "toggle_tag_type.hpp"
#include "toggle_tag.hpp"
#include "collision_user_data.hpp"
#include "ball.hpp"
#include "game_accessor.hpp"

struct toggle_tag;
struct ball;

struct toggle
{
    static thread_local std::size_t id_counter;
    std::size_t id;
    game_accessor game;
    float x;
    float y;
    std::uint32_t timer;
    std::vector<toggle_tag> tags;
    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;

    toggle();
    toggle(
        const float x,
        const float y,
        const std::uint32_t timer,
        const std::vector<toggle_tag> tags
    );
    ~toggle();

    void add_to_world(b2World * world);

    void step_on(ball* m);
    void step_off(ball* m);
};

void to_json(nlohmann::json& j, const toggle& p);
void from_json(const nlohmann::json& j, toggle& p);

#endif

