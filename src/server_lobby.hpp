#ifndef ML_SERVER_LOBBY_HPP
#define ML_SERVER_LOBBY_HPP

#include <vector>
#include <thread>
#include <functional>
#include <memory>
#include "libs/json.hpp"
#include "map.hpp"
#include "game.hpp"
#include "websocket_server.hpp"
#include "settings.hpp"

struct server_lobby
{
    std::vector<std::unique_ptr<game>> games;
    bool is_initialized;
    bool is_alive;

    // use this to initialize
    static server_lobby& get_instance();

    // prevent copies
    server_lobby(server_lobby const&) = delete;
    // prevent move which could invalidate state
    server_lobby(server_lobby&&) = delete;

    void start_server();

    game& get_game_from_port(const std::uint16_t port) const;
private:
    // prevent client from creating new instances
    server_lobby() {}
};

#endif

