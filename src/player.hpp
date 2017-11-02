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
    bool local;
    websocketpp::connection_hdl con;
    server* srv;
    game* g;
    ball* b;
    std::string player_id;
    bool is_registered;
    std::string name;
    int degree;

    int xdir;
    int ydir;

    player(
        const websocketpp::connection_hdl con,
        server* srv,
        game* g,
        ball* b,
        const std::string player_id,
        const bool is_registered,
        const std::string name,
        const int degree
    )
    : local(false)
    , con(con)
    , srv(srv)
    , g(g)
    , b(b)
    , player_id(player_id)
    , is_registered(is_registered)
    , name(name)
    , degree(degree)
    , xdir(0)
    , ydir(0)
    {}

    // for "bots"
    player(
        game* g,
        ball* b,
        const std::string player_id,
        const bool is_registered,
        const std::string name,
        const int degree
    )
    : local(true)
    , con()
    , srv(nullptr)
    , g(g)
    , b(b)
    , player_id(player_id)
    , is_registered(is_registered)
    , name(name)
    , degree(degree)
    , xdir(0)
    , ydir(0)
    {}
};

#endif

