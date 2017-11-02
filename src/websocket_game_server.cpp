#include "websocket_game_server.hpp"

int start_game_server(const std::uint16_t port) 
{
    spdlog::get("game")->info("starting tagos game server on port: {0:d}", port);
    server srv;

    try {
        srv.set_access_channels(websocketpp::log::alevel::all);
        srv.clear_access_channels(websocketpp::log::alevel::frame_payload);

        srv.init_asio();
        srv.set_message_handler(bind(&handle_game_message,&srv,::_1,::_2));
        srv.listen(port);

        srv.start_accept();
        srv.run();
    } catch (websocketpp::exception const & e) {
        spdlog::get("game")->error("server exception", e.what());
        return 1;
    } catch (...) {
        spdlog::get("game")->error("unknown server exception");
        return 1;
    }

    return 0;
}


void handle_game_message(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg
) {
    try {
        nlohmann::json j = nlohmann::json::parse(msg->get_payload());

        if(j.find("request") != j.end()) {
            const std::string req = j.at("request").get<std::string>();

            if(req == "gamesync") {
                const std::string login_token = j.at("login_token").get<std::string>();
                return on_game_sync(srv, hdl, msg, login_token);
            }

            if(req == "chat") {
                const std::string chat_msg = j.at("msg").get<std::string>();
                return on_game_chat(srv, hdl, msg, chat_msg);
            }
        } else {
            try_send(srv, hdl, websocketpp::frame::opcode::value::text, {
                {"error", "missing_request"}
            });
        }
    } catch(...) {
        try_send(srv, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "json_parse_error"}
        });
    }
} 

void on_game_chat(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg,
    const std::string& chat_msg
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(get_local_port(srv, hdl));
    player* p = g.get_player_from_con(hdl);

    if(p) {
        g.add_client_action(client_action(client_action_chat(p, chat_msg)));
    } else {
        spdlog::get("game")->debug("player chatted but hasnt joined yet");
        try_send(srv, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "not_joined"}
        });
    }
}

void on_game_sync(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg,
    const std::string& login_token
) {
    const lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(get_local_port(srv, hdl));

    /* get login_token
     * so we can get user_id, name, degrees
     * respond with map id
     */
    // user o = something(login_token);


    // check game still has open slot
    if(g.m->balls.size() >= 8) {
        try_send(srv, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "game_full"}
        });
        return;
    }

    // we send all map data here to sync user
    try_send(srv, hdl, websocketpp::frame::opcode::value::text, 
        game_event(game_event_gamesync(g))
    );

    // add ball & player to game
    // select ball color
    const ball_type color = [](const game& g){
        std::size_t red_cnt  = 0;
        std::size_t blue_cnt = 0;

        for(auto && o : g.players) {
            if(o->b->type == ball_type::red)  ++red_cnt;
            if(o->b->type == ball_type::blue) ++blue_cnt;
        }

        if(red_cnt > red_cnt)  return ball_type::blue;
        if(red_cnt < blue_cnt) return ball_type::red;

        return std::uniform_int_distribution<int>(0, 1)(random_util::get_instance().eng) == 0
            ? ball_type::red
            : ball_type::blue;
    }(g);

    ball* b = g.add_ball(ball(ball_type::red));
    player* p = g.add_player(player(hdl, srv, &g, b, "player_id", true, "name", 100));
    b->set_player_ptr(p);

    try_broadcast(&g, game_event(game_event_player_joined(p)));
}

