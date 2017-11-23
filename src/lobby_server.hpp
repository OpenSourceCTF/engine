#ifndef ML_SERVER_LOBBY_HPP
#define ML_SERVER_LOBBY_HPP

#include <vector>
#include <thread>
#include <functional>
#include <memory>
#include <json/json_fwd.hpp>
#include "map.hpp"
#include "game.hpp"
#include "websocket_lobby_server.hpp"
#include "settings.hpp"
#include "lobby_event.hpp"

struct game;

struct lobby_server
{
    std::vector<std::unique_ptr<game>> games;
    bool is_initialized;
    bool is_alive;
    std::thread srv_thread;
    websocket_lobby_server* srv;

    // use this to initialize
    static lobby_server& get_instance();

    // prevent copies
    lobby_server(lobby_server const&) = delete;
    // prevent move which could invalidate state
    lobby_server(lobby_server&&) = delete;

    ~lobby_server();

    void start_server();
    void run_server();
    void close();

    game& get_game_from_port(const std::uint16_t port) const;
    std::vector<lobby_event_games_game> get_games() const;
private:
    // prevent client from creating new instances
    lobby_server() {}
};

#endif

