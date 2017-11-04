#include "player.hpp"

player::player(
    const websocketpp::connection_hdl con,
    websocketpp::server<websocketpp::config::asio>* srv,
    game* g,
    ball* b,
    const std::string player_id,
    const bool is_registered,
    const std::string name,
    const int degree
)
: local(false)
, remove(false)
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
player::player(
    game* g,
    ball* b,
    const std::string player_id,
    const bool is_registered,
    const std::string name,
    const int degree
)
: local(true)
, remove(false)
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

player::~player()
{
    if(b) {
        if(b->is_alive) {
            b->pop();
        }

        // delete corresponding ball from list
        g->m->balls.erase(
            std::remove_if(
                g->m->balls.begin(),
                g->m->balls.end(),
                [&](std::unique_ptr<ball> & o) {
                    return o.get() == b;
                }
            ),
            g->m->balls.end()
        );
    }
}
