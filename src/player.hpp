#ifndef ML_PLAYER_HPP
#define ML_PLAYER_HPP

#include <string>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "game.hpp"
#include "ball.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;

struct game;

struct player
{
    websocketpp::connection_hdl con;
    server* srv;
    game* g;
    ball* b;
    std::string player_id;
    std::string name;
    int degree;

    player(
        const websocketpp::connection_hdl con,
        server* srv,
        game* g,
        ball* b,
        const std::string player_id,
        const std::string name,
        const int degree
    )
    : con(con)
    , srv(srv)
    , g(g)
    , b(b)
    , player_id(player_id)
    , name(name)
    , degree(degree)
    {}
};

#endif

