#include "websocket_client.hpp"
#include <json/json.hpp>

bool try_send(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::frame::opcode::value opcode,
    nlohmann::json try_msg
) {
    try {
        c->send(
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
