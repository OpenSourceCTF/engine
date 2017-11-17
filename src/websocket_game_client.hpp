#ifndef ML_WEBSOCKET_GAME_CLIENT_HPP
#define ML_WEBSOCKET_GAME_CLIENT_HPP

#include <cstdint>
#include <string>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>

#include "websocket_client.hpp"
#include "game_event.hpp"
#include "player.hpp"
#include "user.hpp"
#include "random_util.hpp"

int start_client_server(const std::string uri);

void handle_client_open(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl
);

void handle_client_close(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl
);

void handle_client_fail(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl
);

bool handle_client_ping(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    std::string str
);

bool handle_client_pong(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    std::string str
);

void handle_client_pong_timeout(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    std::string str
);

void handle_client_message(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::client<websocketpp::config::asio>::message_ptr msg
);

void on_client_sync(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::client<websocketpp::config::asio>::message_ptr msg
);

#endif

