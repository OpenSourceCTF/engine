#ifndef ML_WEBSOCKET_LOBBY_SERVER_HPP
#define ML_WEBSOCKET_LOBBY_SERVER_HPP

#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "websocket_server.hpp"
#include "lobby_event.hpp"
#include "lobby_server.hpp"

typedef websocketpp::server<websocketpp::config::asio> websocketpp_server;

struct lobby_server;

int start_lobby_server(lobby_server& lobby, const std::uint16_t port);

void handle_lobby_open(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

void handle_lobby_close(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

void handle_lobby_fail(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

bool handle_lobby_ping(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    std::string str
);

bool handle_lobby_pong(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    std::string str
);

void handle_lobby_pong_timeout(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    std::string str
);

void handle_lobby_interrupt(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

bool handle_lobby_validate(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);

void handle_lobby_message(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg
);
        
void handle_lobby_http(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl
);
        
void on_lobby_request_games(
    websocketpp_server* srv,
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg
);

#endif

