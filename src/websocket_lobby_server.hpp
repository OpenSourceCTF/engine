#ifndef ML_WEBSOCKET_LOBBY_SERVER_HPP
#define ML_WEBSOCKET_LOBBY_SERVER_HPP

#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "websocket_server.hpp"
#include "lobby_event.hpp"
#include "lobby_server.hpp"

struct lobby_server;

void handle_lobby_message(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg
);
        
int start_lobby_server(lobby_server& lobby, const std::uint16_t port);

#endif

