#include "websocket_game_server.hpp"

int start_game_server(const std::uint16_t port) 
{
    spdlog::get("game")->info("starting tagos game server on port: {0:d}", port);
    websocketpp::server<websocketpp::config::asio> srv;

    try {
        srv.set_reuse_addr(true);

        srv.clear_access_channels(websocketpp::log::alevel::all);
        srv.set_access_channels(websocketpp::log::elevel::info);

        srv.set_open_handler(bind(&handle_game_open, &srv, ::_1));
        srv.set_close_handler(bind(&handle_game_close, &srv, ::_1));
        srv.set_fail_handler(bind(&handle_game_fail, &srv, ::_1));
        srv.set_ping_handler(bind(&handle_game_ping, &srv, ::_1, ::_2));
        srv.set_pong_handler(bind(&handle_game_pong, &srv, ::_1, ::_2));
        srv.set_pong_timeout_handler(bind(&handle_game_pong_timeout, &srv, ::_1, ::_2));
        srv.set_interrupt_handler(bind(&handle_game_interrupt, &srv, ::_1));
        srv.set_validate_handler(bind(&handle_game_validate, &srv, ::_1));
        srv.set_message_handler(bind(&handle_game_message, &srv, ::_1, ::_2));
        srv.set_http_handler(bind(&handle_game_http, &srv, ::_1));

        srv.init_asio();
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

void handle_game_open(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
) {}

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

void handle_game_fail(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
) {}

bool handle_game_ping(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    std::string str
) {
    return true;
}

bool handle_game_pong(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    std::string str
) {
    return true;
}

void handle_game_pong_timeout(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl,
    std::string str
) {}

void handle_game_interrupt(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
) {}

bool handle_game_validate(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
) {
    // todo
    // validate if connection is valid/allowed
    
    return true;
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

void handle_game_http(
    websocketpp::server<websocketpp::config::asio>* srv,
    websocketpp::connection_hdl hdl
) {
    auto con = srv->get_con_from_hdl(hdl);

	con->set_status(websocketpp::http::status_code::ok);
	con->set_body("Hello World!");
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

    // todo
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

    // add ball & player to game
    ball* b = g.add_ball(new ball(selected_team_color));
    player* p = g.add_player(new player(hdl, srv, &g, b, "player_id", true, "name", 100));
    b->set_player_ptr(p);

    // we send all map data here to sync user
    g.add_server_event(server_event(server_event_gamesync(p)));

    // and let everyone know a new player has joined
    g.add_server_event(server_event(server_event_player_joined(p)));
}

