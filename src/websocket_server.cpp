#include "websocket_server.hpp"

void on_message(
    server* srv,
    websocketpp::connection_hdl hdl,
    message_ptr msg
) {
    std::cout
        << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg->get_payload()
        << std::endl;

    /*
    // clean exit-->
    if (msg->get_payload() == "stop-listening") {
        srv->stop_listening();
        return;
    }*/

    try {
        srv->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (const websocketpp::lib::error_code& e) {
        std::cerr
            << "Echo failed because: " << e
            << "(" << e.message() << ")"
            << std::endl;
    }
}

int start_server(const std::uint16_t port) 
{
    std::cout << "starting tagos server" << std::endl;
    server srv;

    try {
        srv.set_access_channels(websocketpp::log::alevel::all);
        srv.clear_access_channels(websocketpp::log::alevel::frame_payload);

        srv.init_asio();
        srv.set_message_handler(bind(&on_message,&srv,::_1,::_2));
        srv.listen(port);

        std::cout << "listening on port: " << port << std::endl;

        srv.start_accept();
        srv.run();
    } catch (websocketpp::exception const & e) {
        std::cerr << "error: server exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "error: unknown server exception" << std::endl;
        return 1;
    }

    return 0;
}

