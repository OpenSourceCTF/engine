#include "websocket_server.hpp"

std::uint16_t get_local_port(
    server* srv,
    websocketpp::connection_hdl hdl
) {
    server::connection_ptr connection = srv->get_con_from_hdl(hdl);
    auto local_endpoint = connection->get_raw_socket().local_endpoint();

    return local_endpoint.port();
}

bool try_send(
    server* srv,
    websocketpp::connection_hdl hdl,
    message_ptr msg,
    nlohmann::json try_msg
) {
    try {
        srv->send(
            hdl,
            try_msg.dump(),
            msg->get_opcode()
        );
    } catch (const websocketpp::lib::error_code& e) {
        std::cerr
            << "Echo failed because: " << e
            << "(" << e.message() << ")"
            << std::endl;
        return false;
    }

    return false;
}

