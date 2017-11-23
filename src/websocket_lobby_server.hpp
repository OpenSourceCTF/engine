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

struct websocket_lobby_server
{
    websocketpp_server endpoint;
    lobby_server& lobby;
    const std::uint16_t port;

    websocket_lobby_server(lobby_server& lobby, const std::uint16_t port);

    int start_server();

    void handle_open(
        websocketpp::connection_hdl hdl
    );

    void handle_close(
        websocketpp::connection_hdl hdl
    );

    void handle_fail(
        websocketpp::connection_hdl hdl
    );

    bool handle_ping(
        websocketpp::connection_hdl hdl,
        std::string str
    );

    bool handle_pong(
        websocketpp::connection_hdl hdl,
        std::string str
    );

    void handle_pong_timeout(
        websocketpp::connection_hdl hdl,
        std::string str
    );

    void handle_interrupt(
        websocketpp::connection_hdl hdl
    );

    bool handle_validate(
        websocketpp::connection_hdl hdl
    );

    void handle_message(
        websocketpp::connection_hdl hdl,
        websocketpp_server::message_ptr msg
    );
            
    void handle_http(
        websocketpp::connection_hdl hdl
    );
            
    void on_request_games(
        websocketpp::connection_hdl hdl,
        websocketpp_server::message_ptr msg
    );
};

#endif

