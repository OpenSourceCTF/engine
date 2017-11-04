#include "websocket_game_server.hpp"

int start_game_server(const std::uint16_t port) 
{
    spdlog::get("game")->info("starting tagos game server on port: {0:d}", port);
    websocketpp::server<websocketpp::config::asio> srv;

    try {
        srv.clear_access_channels(websocketpp::log::alevel::all);
        srv.set_access_channels(websocketpp::log::elevel::info);
        srv.init_asio();
        srv.set_message_handler(bind(&handle_game_message, &srv, ::_1, ::_2));
        srv.set_close_handler(bind(&handle_game_close, &srv, ::_1));
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

            } else if(req == "chat") {
                const std::string chat_msg = j.at("msg").get<std::string>();
                return on_game_chat(srv, hdl, msg, chat_msg);

            } else if(req == "teamchat") {
                const std::string chat_msg = j.at("msg").get<std::string>();
                return on_game_teamchat(srv, hdl, msg, chat_msg);

            } else if(req == "movement") {
                const int xdir = j.at("xdir").get<int>();
                const int ydir = j.at("ydir").get<int>();
                return on_game_movement(srv, hdl, msg, xdir, ydir);

            } else if(req == "honk") {
                return on_game_honk(srv, hdl, msg);

            } else {
                try_send(srv, hdl, websocketpp::frame::opcode::value::text, {
                    {"error", "unknown_request"}
                });
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

void handle_game_close(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(get_local_port(srv, hdl));

    player* p = g.get_player_from_con(hdl);

    if(p && ! p->remove) {
        p->local = true;
        p->remove = true;

        g.add_server_event(server_event(server_event_player_left(p)));
    } else {
        spdlog::get("game")->debug("player left but already left?");
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
        g.add_server_event(server_event(server_event_chat(p, chat_msg)));
    } else {
        spdlog::get("game")->debug("player chatted but hasnt joined yet");
        try_send(srv, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "not_joined"}
        });
    }
}

void on_game_teamchat(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg,
    const std::string& chat_msg
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(get_local_port(srv, hdl));
    player* p = g.get_player_from_con(hdl);

    if(p) {
        g.add_server_event(server_event(server_event_teamchat(p, chat_msg)));
    } else {
        spdlog::get("game")->debug("player team chatted but hasnt joined yet");
        try_send(srv, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "not_joined"}
        });
    }
}

void on_game_movement(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg,
    const int xdir,
    const int ydir
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(get_local_port(srv, hdl));
    player* p = g.get_player_from_con(hdl);

    if(p) {
        g.add_server_event(server_event(server_event_movement(p, xdir, ydir)));
    } else {
        spdlog::get("game")->debug("player movement but hasnt joined yet");
        try_send(srv, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "not_joined"}
        });
    }
}

void on_game_honk(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(get_local_port(srv, hdl));
    player* p = g.get_player_from_con(hdl);

    if(p) {
        g.add_server_event(server_event(server_event_honk(p)));
    } else {
        spdlog::get("game")->debug("player honked but hasnt joined yet");
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
    // todo: should this be moved to a server_event ?
    try_send(srv, hdl, websocketpp::frame::opcode::value::text, 
        game_event(game_event_gamesync(g))
    );

    // add ball & player to game
    // select ball color
    const ball_type selected_team_color = [](const game& g){
        std::size_t red_cnt  = 0;
        std::size_t blue_cnt = 0;

        for(auto && o : g.players) {
            if(o->b->type == ball_type::red)  ++red_cnt;
            if(o->b->type == ball_type::blue) ++blue_cnt;
        }

        if(red_cnt > blue_cnt) return ball_type::blue;
        if(red_cnt < blue_cnt) return ball_type::red;

        return std::uniform_int_distribution<int>(0, 1)(random_util::get_instance().eng) == 0
            ? ball_type::red
            : ball_type::blue;
    }(g);

    ball* b = g.add_ball(new ball(selected_team_color));
    player* p = g.add_player(new player(hdl, srv, &g, b, "player_id", true, "name", 100));
    b->set_player_ptr(p);

    g.add_server_event(server_event(server_event_player_joined(p)));
}

