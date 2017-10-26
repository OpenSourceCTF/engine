#ifndef ML_WEBSOCKET_SERVER_HPP
#define ML_WEBSOCKET_SERVER_HPP

#include <iostream>
#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "server_lobby.hpp"
#include "user.hpp"
#include "request_lobby_games_response.hpp"

struct server_lobby;

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void handle_game_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg);
void handle_lobby_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg);

int start_game_server(const std::uint16_t port);
int start_lobby_server(server_lobby& lobby, const std::uint16_t port);


#endif

