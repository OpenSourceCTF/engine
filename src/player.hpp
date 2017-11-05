#ifndef ML_PLAYER_HPP
#define ML_PLAYER_HPP

#include <string>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "game.hpp"
#include "ball.hpp"

struct game;
struct ball;

struct player
{
    bool local;
    bool remove;
    websocketpp::connection_hdl con;
    websocketpp::server<websocketpp::config::asio>* srv;
    game* g;
    ball* b;
    std::string player_id;
    bool is_registered;
    std::string name;
    int degree;

    int xdir;
    int ydir;

    struct {
        std::size_t popped;             // times youve been popped
        std::size_t popped_tagged;      // times youve been tagged
        std::size_t popped_spikes;      // spikes youve hit
        std::size_t popped_gates;       // gates youve been struck down by // todo
        std::size_t tags;               // times youve tagged someone
        std::size_t flag_grabs;         // total flag grabs
        std::size_t flag_drops;         // total flag drops
        std::size_t flag_hold;          // tics you've held flag // todo
        std::size_t flag_returns;       // total pops of enemy flag holder
        std::size_t flag_captures;      // total caps
        std::size_t button_holder;      // tics you've been on a button // todo
        std::size_t powerups_picked_up; // powerups you've picked up
        std::size_t boosters_hit;       // times youve hit a booster
        std::size_t bombs_hit;          // times youve hit a bombs
        std::size_t portals_hit;        // times youve hit a portal
    } stats;

    player(
        const websocketpp::connection_hdl con,
        websocketpp::server<websocketpp::config::asio>* srv,
        game* g,
        ball* b,
        const std::string player_id,
        const bool is_registered,
        const std::string name,
        const int degree
    );

    // for "bots"
    player(
        game* g,
        ball* b,
        const std::string player_id,
        const bool is_registered,
        const std::string name,
        const int degree
    );

    ~player();
};

#endif
