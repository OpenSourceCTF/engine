#ifndef ML_WEBSOCKET_SERVER_HPP
#define ML_WEBSOCKET_SERVER_HPP

#include <iostream>
#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "game.hpp"

struct game;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

std::uint16_t get_local_port(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
);

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

#endif

