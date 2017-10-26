#ifndef ML_GAME_HPP
#define ML_GAME_HPP

#include <cstdint>
#include <chrono>
#include <thread>
#include "libs/json.hpp"
#include "map.hpp"

struct game
{
    std::uint16_t port;
    map* m;
    std::uint32_t max_points;
    std::uint32_t max_length;
    b2World * world;
    std::size_t timestep;

    game(){}
    game(const std::uint16_t port, map* m);

    std::thread spawn_thread();
    void run();
    void step();

    b2World* init_world();
    ball* add_ball(b2World* world, ball b);
    void respawn_ball(ball* b);
};

void to_json(nlohmann::json& j, const game& p);

#endif

