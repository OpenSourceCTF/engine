#ifndef ML_WEBSOCKET_GAME_SERVER_HPP
#define ML_WEBSOCKET_GAME_SERVER_HPP

#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "websocket_server.hpp"
#include "lobby_server.hpp"
#include "game_event.hpp"
#include "player.hpp"
#include "user.hpp"
#include "random_util.hpp"

typedef websocketpp::server<websocketpp::config::asio> websocketpp_server;

struct lobby_server;

int start_game_server(const std::uint16_t port);

void handle_game_open(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

void handle_game_close(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

void handle_game_fail(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

bool handle_game_ping(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    std::string str
);

bool handle_game_pong(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    std::string str
);

void handle_game_pong_timeout(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    std::string str
);

void handle_game_interrupt(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

bool handle_game_validate(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

void handle_game_message(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg
);

void handle_game_http(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

void on_game_chat(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg,
    const std::string& chat_msg
);

void on_game_teamchat(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg,
    const std::string& chat_msg
);

void on_game_movement(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg,
    const int xdir,
    const int ydir
);

void on_game_honk(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg
);

void on_game_stats(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg
);

void on_game_sync(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg,
    const std::string& login_token
);

void on_game_vote_player(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg,
    const std::string & player_id,
    const std::string & reason
);

#endif

