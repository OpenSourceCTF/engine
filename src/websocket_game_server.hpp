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
#include "settings.hpp"

typedef websocketpp::server<websocketpp::config::asio> websocketpp_server;

struct lobby_server;

struct websocket_game_server
{
    websocketpp_server endpoint;
    std::uint16_t port;

    websocket_game_server(const std::uint16_t port);

    int start_server();

    void hangle_open(
        websocketpp::connection_hdl hdl
    );

    void hangle_close(
        websocketpp::connection_hdl hdl
    );

    void hangle_fail(
        websocketpp::connection_hdl hdl
    );

    bool hangle_ping(
        websocketpp::connection_hdl hdl,
        std::string str
    );

    bool hangle_pong(
        websocketpp::connection_hdl hdl,
        std::string str
    );

    void hangle_pong_timeout(
        websocketpp::connection_hdl hdl,
        std::string str
    );

    void hangle_interrupt(
        websocketpp::connection_hdl hdl
    );

    bool hangle_validate(
        websocketpp::connection_hdl hdl
    );

    void hangle_message(
        websocketpp::connection_hdl hdl,
        websocketpp_server::message_ptr msg
    );

    void hangle_http(
        websocketpp::connection_hdl hdl
    );

    void on_chat(
        websocketpp::connection_hdl hdl,
        websocketpp_server::message_ptr msg,
        const std::string& chat_msg
    );

    void on_teamchat(
        websocketpp::connection_hdl hdl,
        websocketpp_server::message_ptr msg,
        const std::string& chat_msg
    );

    void on_movement(
        websocketpp::connection_hdl hdl,
        websocketpp_server::message_ptr msg,
        const int xdir,
        const int ydir
    );

    void on_honk(
        websocketpp::connection_hdl hdl,
        websocketpp_server::message_ptr msg
    );

    void on_stats(
        websocketpp::connection_hdl hdl,
        websocketpp_server::message_ptr msg
    );

    void on_sync(
        websocketpp::connection_hdl hdl,
        websocketpp_server::message_ptr msg,
        const std::string& login_token
    );

    void on_vote_player(
        websocketpp::connection_hdl hdl,
        websocketpp_server::message_ptr msg,
        const std::string & player_id,
        const std::string & reason
    );
};

#endif

