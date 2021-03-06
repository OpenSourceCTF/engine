#ifndef ML_WEBSOCKET_SERVER_HPP
#define ML_WEBSOCKET_SERVER_HPP

#include <iostream>
#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "game.hpp"
#include "ball_type.hpp"

struct game;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

bool try_send(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::frame::opcode::value opcode,
    nlohmann::json try_msg
); 

bool try_broadcast(
    game* g,
    nlohmann::json try_msg
);

bool try_broadcast_team(
    game* g,
    ball_type team,
    nlohmann::json try_msg
);

#endif

