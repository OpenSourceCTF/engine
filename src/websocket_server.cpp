#include "websocket_server.hpp"

std::uint16_t get_local_port(
    server* srv,
    websocketpp::connection_hdl hdl
) {
    server::connection_ptr connection = srv->get_con_from_hdl(hdl);
    auto local_endpoint = connection->get_raw_socket().local_endpoint();

    return local_endpoint.port();
}

