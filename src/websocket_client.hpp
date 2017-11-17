#ifndef ML_WEBSOCKET_CLIENT_HPP
#define ML_WEBSOCKET_CLIENT_HPP

#include <iostream>
#include <cstdint>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>
#include <json/json_fwd.hpp>
#include <spdlog/spdlog.h>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

bool try_send(
    websocketpp::client<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::frame::opcode::value opcode,
    nlohmann::json try_msg
); 

#endif


