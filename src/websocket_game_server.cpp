#include "websocket_game_server.hpp"
#include <json/json.hpp>

websocket_game_server::websocket_game_server(const std::uint16_t port)
: port(port)
{}

int websocket_game_server::start_server() 
{
    spdlog::get("game")->info("starting tagos game server on port: {0:d}", port);

    try {
        endpoint.set_reuse_addr(true);

        endpoint.clear_access_channels(websocketpp::log::alevel::all);
        endpoint.set_access_channels(websocketpp::log::elevel::info);

        endpoint.set_open_handler(bind(&websocket_game_server::hangle_open, this, ::_1));
        endpoint.set_close_handler(bind(&websocket_game_server::hangle_close, this, ::_1));
        endpoint.set_fail_handler(bind(&websocket_game_server::hangle_fail, this, ::_1));
        endpoint.set_ping_handler(bind(&websocket_game_server::hangle_ping, this, ::_1, ::_2));
        endpoint.set_pong_handler(bind(&websocket_game_server::hangle_pong, this, ::_1, ::_2));
        endpoint.set_pong_timeout_handler(bind(&websocket_game_server::hangle_pong_timeout, this, ::_1, ::_2));
        endpoint.set_interrupt_handler(bind(&websocket_game_server::hangle_interrupt, this, ::_1));
        endpoint.set_validate_handler(bind(&websocket_game_server::hangle_validate, this, ::_1));
        endpoint.set_message_handler(bind(&websocket_game_server::hangle_message, this, ::_1, ::_2));
        endpoint.set_http_handler(bind(&websocket_game_server::hangle_http, this, ::_1));

        endpoint.init_asio();
        endpoint.listen(port);
        endpoint.start_accept();
        endpoint.run();
    } catch (websocketpp::exception const & e) {
        spdlog::get("game")->error("server exception {}", e.what());
        std::exit(EXIT_FAILURE);
        return 1;
    } catch (...) {
        spdlog::get("game")->error("unknown server exception");
        std::exit(EXIT_FAILURE);
        return 1;
    }

    return 0;
}

void websocket_game_server::websocket_game_server::hangle_open(
    websocketpp::connection_hdl hdl
) {
    spdlog::get("game")->info("game open");
}

void websocket_game_server::websocket_game_server::hangle_close(
    websocketpp::connection_hdl hdl
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(port);

    player* p = g.get_player_from_con(hdl);

    if(p && ! p->remove) {
        p->local = true;
        p->remove = true;

        g.add_server_event(server_event(server_event_player_left(p)));
    } else {
        spdlog::get("game")->debug("player left but already left?");
    }
    endpoint.pause_reading(hdl);
    spdlog::get("game")->info("game close");
}

void websocket_game_server::websocket_game_server::hangle_fail(
    websocketpp::connection_hdl hdl
) {
    spdlog::get("game")->info("game fail");
}

bool websocket_game_server::hangle_ping(
    websocketpp::connection_hdl hdl,
    std::string str
) {
    spdlog::get("game")->info("game ping");
    return true;
}

bool websocket_game_server::hangle_pong(
    websocketpp::connection_hdl hdl,
    std::string str
) {
    spdlog::get("game")->info("game pong");
    return true;
}

void websocket_game_server::hangle_pong_timeout(
    websocketpp::connection_hdl hdl,
    std::string str
) {
    spdlog::get("game")->info("game pong timeout");
}

void websocket_game_server::hangle_interrupt(
    websocketpp::connection_hdl hdl
) {
    spdlog::get("game")->info("game interrupt");
}

bool websocket_game_server::hangle_validate(
    websocketpp::connection_hdl hdl
) {
    spdlog::get("game")->info("game validate");
    // todo
    // validate if connection is valid/allowed
    
    return true;
}

void websocket_game_server::hangle_message(
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg
) {
    try {
        nlohmann::json j = nlohmann::json::parse(msg->get_payload());

        if(j.find("request") != j.end()) {
            const std::string req = j.at("request").get<std::string>();

            if(req == "gamesync") {
                const std::string login_token = j.at("login_token").get<std::string>();
                return on_sync(hdl, msg, login_token);

            } else if(req == "chat") {
                const std::string chat_msg = j.at("msg").get<std::string>();
                return on_chat(hdl, msg, chat_msg);

            } else if(req == "teamchat") {
                const std::string chat_msg = j.at("msg").get<std::string>();
                return on_teamchat(hdl, msg, chat_msg);

            } else if(req == "movement") {
                const int xdir = j.at("xdir").get<int>();
                const int ydir = j.at("ydir").get<int>();
                return on_movement(hdl, msg, xdir, ydir);

            } else if(req == "honk") {
                return on_honk(hdl, msg);

            } else if(req == "stats") {
                return on_stats(hdl, msg);

            } else if(req == "vote_player") {
                const std::string player_id = j.at("player_id").get<std::string>();
                const std::string reason = j.at("reason").get<std::string>();
                return on_vote_player(hdl, msg, player_id, reason);

            } else {
                try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
                    {"error", "unknown_request"}
                });
            }
        } else {
            try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
                {"error", "missing_request"}
            });
        }
    } catch(...) {
        try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "json_parse_error"}
        });
    }
} 

void websocket_game_server::hangle_http(
    websocketpp::connection_hdl hdl
) {
    auto con = endpoint.get_con_from_hdl(hdl);

	con->set_status(websocketpp::http::status_code::ok);
	con->set_body("Hello World!");
}

void websocket_game_server::on_chat(
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg,
    const std::string& chat_msg
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(port);
    player* p = g.get_player_from_con(hdl);

    if(p) {
        g.add_server_event(server_event(server_event_chat(p, chat_msg)));
    } else {
        spdlog::get("game")->debug("player chatted but hasnt joined yet");
        try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "not_joined"}
        });
    }
}

void websocket_game_server::on_teamchat(
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg,
    const std::string& chat_msg
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(port);
    player* p = g.get_player_from_con(hdl);

    if(p) {
        g.add_server_event(server_event(server_event_teamchat(p, chat_msg)));
    } else {
        spdlog::get("game")->debug("player team chatted but hasnt joined yet");
        try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "not_joined"}
        });
    }
}

void websocket_game_server::on_movement(
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg,
    const int xdir,
    const int ydir
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(port);
    player* p = g.get_player_from_con(hdl);

    if(p) {
        g.add_server_event(server_event(server_event_movement(p, xdir, ydir)));
    } else {
        spdlog::get("game")->debug("player movement but hasnt joined yet");
        try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "not_joined"}
        });
    }
}

void websocket_game_server::on_honk(
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(port);
    player* p = g.get_player_from_con(hdl);

    if(p) {
        g.add_server_event(server_event(server_event_honk(p)));
    } else {
        spdlog::get("game")->debug("player honked but hasnt joined yet");
        try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "not_joined"}
        });
    }
}

void websocket_game_server::on_stats(
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(port);
    player* p = g.get_player_from_con(hdl);

    if(p) {
        g.add_server_event(server_event(server_event_game_stats(p)));
    } else {
        spdlog::get("game")->debug("player requested stats but hasnt joined yet");
        try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "not_joined"}
        });
    }
}

void websocket_game_server::on_sync(
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg,
    const std::string& login_token
) {
    const settings& config = settings::get_instance();
    const lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(port);

    // todo
    /* get login_token
     * so we can get user_id, name, degrees
     * respond with map id
     */
    // user o = something(login_token);


    // check game still has open slot
    if(g.players.size() >= config.GAME_MAX_PLAYERS) {
        try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "game_full"}
        });
        return;
    }

    // and let everyone know a new player has joined
    g.add_server_event(server_event(server_event_player_joined(new player(hdl, &endpoint, &g, "player_id", true, "name", 100))));
}

void websocket_game_server::on_vote_player(
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg,
    const std::string & player_id,
    const std::string & reason
) {
    lobby_server& lobby = lobby_server::get_instance();

    game& g = lobby.get_game_from_port(port);
    player* p = g.get_player_from_con(hdl);
    player* voted_p = g.get_player_from_player_id(player_id);

    if(! p) {
        spdlog::get("game")->debug("player voted but hasnt joined yet");
        try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "not_joined"}
        });
        return;
    }

    if(! voted_p) {
        spdlog::get("game")->debug("player voted player which doesnt exist");
        try_send(&endpoint, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "voted_player_not_exist"}
        });
        return;
    }

    g.add_server_event(server_event(server_event_vote_player(p, voted_p, reason)));
}

