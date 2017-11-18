#include "websocket_game_client.hpp"
#include <json/json.hpp>

int start_client_server(const std::string uri) 
{
    spdlog::get("game")->info("starting tagos client: {}", uri);
    websocketpp_client c;

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
        websocketpp_client::connection_ptr con = c.get_connection(uri, ec);

        c.connect(con);
        c.run();
    } catch (websocketpp::exception const & e) {
        spdlog::get("game")->error("client exception {}", e.what());
        std::exit(EXIT_FAILURE);
        return 1;
    } catch (...) {
        spdlog::get("game")->error("unknown client exception");
        std::exit(EXIT_FAILURE);
        return 1;
    }

    return 0;
}

void handle_client_open(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl
) {
    spdlog::get("game")->info("game open");
}

void handle_client_close(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl
) {}

void handle_client_fail(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl
) {
    spdlog::get("game")->info("game fail");
}

bool handle_client_ping(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl,
    std::string str
) {
    spdlog::get("game")->info("game ping");
    return true;
}

bool handle_client_pong(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl,
    std::string str
) {
    spdlog::get("game")->info("game pong");
    return true;
}

void handle_client_pong_timeout(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl,
    std::string str
) {
    spdlog::get("game")->info("game pong timeout");
}

void handle_client_message(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl,
    websocketpp_client::message_ptr msg
) {
    try {
        nlohmann::json j = nlohmann::json::parse(msg->get_payload());

        if(j.find("event") != j.end()) {
            const std::string evt = j.at("event").get<std::string>();

            // todo
            // fill this out with all the game_event_type types
            if(evt == "gamesync") {
                on_client_sync(c, hdl, msg);
            } else {
                spdlog::get("game")->info("received unknown event: {}", evt);
            }
        } else {
            spdlog::get("game")->info("didn't receive event");
        }
    } catch(...) {
        spdlog::get("game")->error("unknown exception encountered in message handler");
    }
}

void on_client_sync(
    websocketpp_client* c,
    websocketpp::connection_hdl hdl,
    websocketpp_client::message_ptr msg
) {
    spdlog::get("game")->info("gamesync not implemented");
}
