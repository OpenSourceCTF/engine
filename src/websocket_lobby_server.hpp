#ifndef ML_WEBSOCKET_LOBBY_SERVER_HPP
#define ML_WEBSOCKET_LOBBY_SERVER_HPP

#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "websocket_server.hpp"
#include "lobby_event.hpp"
#include "lobby_server.hpp"

struct lobby_server;

int start_lobby_server(lobby_server& lobby, const std::uint16_t port);

void handle_lobby_open(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
);

void handle_lobby_close(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
);

void handle_lobby_fail(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
);

bool handle_lobby_ping(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    std::string str
);

bool handle_lobby_pong(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    std::string str
);

void handle_lobby_pong_timeout(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    std::string str
);

void handle_lobby_interrupt(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
);

bool handle_lobby_validate(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
);

void handle_lobby_message(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg
);
        
void handle_lobby_http(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
);
        
void on_lobby_request_games(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg
);

#endif

