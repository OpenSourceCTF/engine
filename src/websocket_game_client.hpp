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

typedef websocketpp::client<websocketpp::config::asio> websocketpp_client;

int start_client_server(const std::string uri);

void handle_client_open(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl
);

void handle_client_close(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl
);

void handle_client_fail(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl
);

bool handle_client_ping(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl,
    std::string str
);

bool handle_client_pong(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl,
    std::string str
);

void handle_client_pong_timeout(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl,
    std::string str
);

void handle_client_message(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl,
    websocketpp_client::message_ptr msg
);

void on_client_sync(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl,
    websocketpp_client::message_ptr msg
);

#endif

