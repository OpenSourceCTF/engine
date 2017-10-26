#ifndef ML_GAME_HPP
#define ML_GAME_HPP

#include <cstdint>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include "libs/json.hpp"
#include "map.hpp"
#include "client_action.hpp"

struct game
{
    std::uint16_t port;
    map* m;
    std::uint32_t max_points;
    std::uint32_t max_length;
    b2World * world;
    std::size_t timestep;
    std::mutex client_actions_queue_mutex;
    std::queue<client_action> client_actions_queue;

    game(){}
    game(const std::uint16_t port, map* m);
    game(const game&) = delete;

    std::thread spawn_thread();
    void run();
    void step();

    b2World* init_world();
    ball* add_ball(b2World* world, ball b);
    void respawn_ball(ball* b);
};

#endif

