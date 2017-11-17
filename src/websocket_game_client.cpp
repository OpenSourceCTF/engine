#include "websocket_game_client.hpp"
#include <json/json.hpp>

int start_client_server(const std::string uri) 
{
    spdlog::get("game")->info("starting tagos client: {}", uri);
    websocketpp::client<websocketpp::config::asio> c;

    try {
        c.set_reuse_addr(true);

        c.clear_access_channels(websocketpp::log::alevel::all);
        c.set_access_channels(websocketpp::log::elevel::info);

        c.set_open_handler(bind(&handle_client_open, &c, ::_1));
        c.set_close_handler(bind(&handle_client_close, &c, ::_1));
        c.set_fail_handler(bind(&handle_client_fail, &c, ::_1));
        c.set_ping_handler(bind(&handle_client_ping, &c, ::_1, ::_2));
        c.set_pong_handler(bind(&handle_client_pong, &c, ::_1, ::_2));
        c.set_pong_timeout_handler(bind(&handle_client_pong_timeout, &c, ::_1, ::_2));
        c.set_message_handler(bind(&handle_client_message, &c, ::_1, ::_2));

        c.init_asio();
        websocketpp::lib::error_code ec;
        websocketpp::client<websocketpp::config::asio>::connection_ptr con = c.get_connection(uri, ec);

        c.connect(con);
        c.run();
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

void handle_client_open(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl
) {
    spdlog::get("game")->info("game open");
}

void handle_client_close(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl
) {}

void handle_client_fail(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl
) {
    spdlog::get("game")->info("game fail");
}

bool handle_client_ping(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    std::string str
) {
    spdlog::get("game")->info("game ping");
    return true;
}

bool handle_client_pong(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    std::string str
) {
    spdlog::get("game")->info("game pong");
    return true;
}

void handle_client_pong_timeout(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    std::string str
) {
    spdlog::get("game")->info("game pong timeout");
}

void handle_client_message(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::client<websocketpp::config::asio>::message_ptr msg
) {
    try {
        nlohmann::json j = nlohmann::json::parse(msg->get_payload());

        if(j.find("request") != j.end()) {
            const std::string req = j.at("request").get<std::string>();

            if(req == "gamesync") {
                const std::string login_token = j.at("login_token").get<std::string>();
                return on_client_sync(c, hdl, msg, login_token);

            } else if(req == "chat") {
                const std::string chat_msg = j.at("msg").get<std::string>();
                return on_client_chat(c, hdl, msg, chat_msg);

            } else if(req == "teamchat") {
                const std::string chat_msg = j.at("msg").get<std::string>();
                return on_client_teamchat(c, hdl, msg, chat_msg);

            } else if(req == "movement") {
                const int xdir = j.at("xdir").get<int>();
                const int ydir = j.at("ydir").get<int>();
                return on_client_movement(c, hdl, msg, xdir, ydir);

            } else if(req == "honk") {
                return on_client_honk(c, hdl, msg);

            } else if(req == "stats") {
                return on_client_stats(c, hdl, msg);

            } else if(req == "vote_player") {
                const std::string player_id = j.at("player_id").get<std::string>();
                const std::string reason = j.at("reason").get<std::string>();
                return on_client_vote_player(c, hdl, msg, player_id, reason);

            } else {
                try_send(c, hdl, websocketpp::frame::opcode::value::text, {
                    {"error", "unknown_request"}
                });
            }
        } else {
            try_send(c, hdl, websocketpp::frame::opcode::value::text, {
                {"error", "missing_request"}
            });
        }
    } catch(...) {
        try_send(c, hdl, websocketpp::frame::opcode::value::text, {
            {"error", "json_parse_error"}
        });
    }
}

void on_client_chat(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::client<websocketpp::config::asio>::message_ptr msg,
    const std::string& chat_msg
) {}

void on_client_teamchat(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::client<websocketpp::config::asio>::message_ptr msg,
    const std::string& chat_msg
) {}

void on_client_movement(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::client<websocketpp::config::asio>::message_ptr msg,
    const int xdir,
    const int ydir
) {}

void on_client_honk(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::client<websocketpp::config::asio>::message_ptr msg
) {}

void on_client_stats(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::client<websocketpp::config::asio>::message_ptr msg
) {}

void on_client_sync(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::client<websocketpp::config::asio>::message_ptr msg,
    const std::string& login_token
) {}

void on_client_vote_player(
    websocketpp::client<websocketpp::config::asio>* c,
    websocketpp::connection_hdl hdl,
    websocketpp::client<websocketpp::config::asio>::message_ptr msg,
    const std::string & player_id,
    const std::string & reason
) {}
