#ifndef ML_WEBSOCKET_SERVER_HPP
#define ML_WEBSOCKET_SERVER_HPP

#include <iostream>
#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "server_lobby.hpp"
#include "player.hpp"
#include "user.hpp"
#include "random_util.hpp"
#include "request_lobby_games_response.hpp"
#include "request_game_sync_response.hpp"
#include "game_event.hpp"

struct server_lobby;

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

std::uint16_t get_local_port(
    server* srv,
    websocketpp::connection_hdl hdl
);

bool try_send(
    server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::frame::opcode::value opcode,
    nlohmann::json try_msg
); 

bool try_broadcast(
    game* g,
    nlohmann::json try_msg
);

// NOTE: these are implemented in websocket_lobby_server and websocket_game_server
// define a callback to handle incoming messages
void handle_game_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg);
void handle_lobby_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg);

int start_game_server(const std::uint16_t port);
int start_lobby_server(server_lobby& lobby, const std::uint16_t port);


void on_lobby_request_games(
    server* srv,
    websocketpp::connection_hdl hdl,
    message_ptr msg
);

void on_game_chat(
    server* srv,
    websocketpp::connection_hdl hdl,
    message_ptr msg,
    const std::string& chat_msg
);

void on_game_sync(
    server* srv,
    websocketpp::connection_hdl hdl,
    message_ptr msg,
    const std::string& login_token
);

#endif

