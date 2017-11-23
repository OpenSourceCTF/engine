#include "websocket_server.hpp"
#include <json/json.hpp>

bool try_send(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::frame::opcode::value opcode,
    nlohmann::json try_msg
) {
    try {
        srv->send(
            hdl,
            try_msg.dump(),
            opcode
        );
    } catch (const websocketpp::lib::error_code& e) {
        spdlog::get("game")->critical(e.message());
        return false;
    }

    return false;
}

bool try_broadcast(
    game* g,
    nlohmann::json try_msg
) {
    bool ret = true;

    for(auto && o : g->players) {
        if(! o->local && ! o->remove) {
            if(! try_send(
                o->srv,
                o->con,
                websocketpp::frame::opcode::value::text,
                try_msg
            )) ret = false;
        }
    }

    return ret;
}

bool try_broadcast_team(
    game* g,
    ball_type team,
    nlohmann::json try_msg
) {
    bool ret = true;

    for(auto && o : g->players) {
        if(! o->local && o->b->type == team) {
            if(! try_send(
                o->srv,
                o->con,
                websocketpp::frame::opcode::value::text,
                try_msg
            )) ret = false;
        }
    }

    return ret;
}
