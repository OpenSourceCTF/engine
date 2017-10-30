#include "websocket_server.hpp"

void on_lobby_request_games(
    server* srv,
    websocketpp::connection_hdl hdl,
    message_ptr msg
) {
    const server_lobby& lobby = server_lobby::get_instance();

    std::vector<request_lobby_games_response> games;
    games.reserve(lobby.games.size());

    for(auto && o : lobby.games) {
        games.emplace_back(
            o->port,
            o->max_points,
            o->max_length,
            o->timestep,
            o->m->name,
            o->m->author,
            o->m->balls.size()
        );
    }

    try_send(srv, hdl, websocketpp::frame::opcode::TEXT, {
        {"games", games}
    });

}

void handle_lobby_message(
    server* srv,
    websocketpp::connection_hdl hdl,
    message_ptr msg
) {
    try {
        nlohmann::json j = nlohmann::json::parse(msg->get_payload());
        // std::cout << j.dump() << std::endl;

        if(j.at("request").get<std::string>() == "games") {
            return on_lobby_request_games(srv, hdl, msg);
        }

    } catch(...) {
        try_send(srv, hdl, websocketpp::frame::opcode::TEXT, {
            {"error", "json_parse_error"}
        });
    }
}

int start_lobby_server(
    server_lobby& lobby,
    const std::uint16_t port
) {
    std::cout << "starting tagos lobby server... ";
    server srv;

    try {
        srv.set_access_channels(websocketpp::log::alevel::all);
        srv.clear_access_channels(websocketpp::log::alevel::frame_payload);

        srv.init_asio();
        srv.set_message_handler(bind(&handle_lobby_message,&srv,::_1,::_2));
        srv.listen(port);

        std::cout << "on port: " << port << std::endl;

        srv.start_accept();
        srv.run();
    } catch (websocketpp::exception const & e) {
        std::cerr << "error: server exception: " << e.what() << std::endl;
        lobby.is_alive = false;
        return 1;
    } catch (...) {
        std::cerr << "error: unknown server exception" << std::endl;
        lobby.is_alive = false;
        return 1;
    }

    return 0;
}


