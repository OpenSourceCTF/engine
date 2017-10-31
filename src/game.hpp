#ifndef ML_GAME_HPP
#define ML_GAME_HPP

#include <cstdint>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <memory>
#include "libs/json.hpp"
#include "map.hpp"
#include "client_action.hpp"
#include "player.hpp"

struct player;
struct map;

struct game
{
    std::uint16_t port;
    map* m;
    std::uint32_t max_points;
    std::uint32_t max_length;
    std::uint32_t red_points;
    std::uint32_t blue_points;
    b2World * world;
    std::size_t timestep;
    std::vector<std::unique_ptr<player>> players;
    std::mutex client_actions_queue_mutex;
    std::queue<client_action> client_actions_queue;

    game(){}
    game(const std::uint16_t port, map* m);
    game(const game&) = delete;

    std::thread spawn_thread();
    void run();
    void step();

    b2World* init_world();
    ball* add_ball(ball b);
    void respawn_ball(ball* b);
    player* add_player(player p);
    void score(ball* b);
};

#endif

