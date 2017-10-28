#include "websocket_server.hpp"

int start_game_server(const std::uint16_t port) 
{
    std::cout << "starting tagos game server... ";
    server srv;

    try {
        srv.set_access_channels(websocketpp::log::alevel::all);
        srv.clear_access_channels(websocketpp::log::alevel::frame_payload);

        srv.init_asio();
        srv.set_message_handler(bind(&handle_game_message,&srv,::_1,::_2));
        srv.listen(port);

        std::cout << "on port: " << port << std::endl;

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


void handle_game_message(
    server* srv,
    websocketpp::connection_hdl hdl,
    message_ptr msg
) {
    try {
        nlohmann::json j = nlohmann::json::parse(msg->get_payload());
        std::cout << j.dump() << std::endl;

        if(j.find("request") != j.end()) {
            const std::string req = j.at("request").get<std::string>();
            std::cout << "req: " << req << std::endl;

            if(req == "gamesync") {
                const std::string login_token = j.at("login_token").get<std::string>();
                return on_game_sync(srv, hdl, msg, login_token);
            }

            if(req == "chat") {
                const std::string chat_msg = j.at("msg").get<std::string>();
                return on_game_chat(srv, hdl, msg, chat_msg);
            }
        } else {
            try_send(srv, hdl, msg, {
                {"error", "missing_request"}
            });
        }
    } catch(...) {
        try_send(srv, hdl, msg, {
            {"error", "json_parse_error"}
        });
    }
} 

void on_game_chat(
    server* srv,
    websocketpp::connection_hdl hdl,
    message_ptr msg,
    const std::string& chat_msg
) {
    server_lobby& lobby = server_lobby::get_instance();

    game& g = lobby.get_game_from_port(get_local_port(srv, hdl));

    // broadcast chat_msg
}

void on_game_sync(
    server* srv,
    websocketpp::connection_hdl hdl,
    message_ptr msg,
    const std::string& login_token
) {
    const server_lobby& lobby = server_lobby::get_instance();

    const game& g = lobby.get_game_from_port(get_local_port(srv, hdl));

    /* get login_token
     * so we can get user_id, name, degrees
     * respond with map id
     */
    // user o = something(login_token);


    if(g.m->balls.size() >= 8) {
        try_send(srv, hdl, msg, {
            {"sync", {"error", "game_full"}}
        });
        return;
    }

    /*
     * check game still has open slot
     * we send all map data here to sync user
     * add clients ball
     * notify all clients on game of new ball
     * save connection hdl/server into stdmap
     */

    try_send(srv, hdl, msg, {
        {"sync", request_game_sync_response(g)} 
    });
}

