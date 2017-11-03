#ifndef ML_GAME_HPP
#define ML_GAME_HPP

#include <cstdint>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <memory>
#include <json.hpp>
#include "map.hpp"
#include "server_event.hpp"
#include "player.hpp"
#include "websocket_game_server.hpp"
#include "ball_type.hpp"

struct player;
struct map;

struct game
{
    std::thread srv_thread;
    std::thread phys_thread;

    std::uint16_t port;
    map* m;
    std::uint32_t max_points;
    std::uint32_t max_length;
    std::uint32_t red_points;
    std::uint32_t blue_points;
    b2World * world;
    std::size_t timestep;
    std::vector<std::unique_ptr<player>> players;
    std::mutex server_events_queue_mutex;
    std::queue<server_event> server_events_queue;

    game(){}
    game(const std::uint16_t port, map* m);
    game(const game&) = delete;

    void spawn_srv_thread();
    void spawn_phys_thread();
    void run();
    void step();
    void handle_server_events();

    b2World* init_world();
    ball* add_ball(ball* b);
    void respawn_ball(ball* b);
    player* add_player(player* p);
    void score(ball* b);
    player* get_player_from_con(websocketpp::connection_hdl con);
    void add_server_event(server_event a);
};

#endif

