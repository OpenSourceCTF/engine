#include "websocket_server.hpp"
#include "websocket_lobby_server.hpp"
#include <json/json.hpp>

websocket_lobby_server::websocket_lobby_server(
    lobby_server& lobby,
    const std::uint16_t port
)
: lobby(lobby)
, port(port)
{}

int websocket_lobby_server::start_server()
{
    spdlog::get("game")->info("starting tagos lobby server on port: {0:d}", port);

    try {
        endpoint.set_reuse_addr(true);

        endpoint.clear_access_channels(websocketpp::log::alevel::all);
        endpoint.set_access_channels(websocketpp::log::elevel::info);

        endpoint.set_open_handler(bind(&websocket_lobby_server::handle_open, this, ::_1));
        endpoint.set_close_handler(bind(&websocket_lobby_server::handle_close, this, ::_1));
        endpoint.set_fail_handler(bind(&websocket_lobby_server::handle_fail, this, ::_1));
        endpoint.set_ping_handler(bind(&websocket_lobby_server::handle_ping, this, ::_1, ::_2));
        endpoint.set_pong_handler(bind(&websocket_lobby_server::handle_pong, this, ::_1, ::_2));
        endpoint.set_pong_timeout_handler(bind(&websocket_lobby_server::handle_pong_timeout, this, ::_1, ::_2));
        endpoint.set_interrupt_handler(bind(&websocket_lobby_server::handle_interrupt, this, ::_1));
        endpoint.set_validate_handler(bind(&websocket_lobby_server::handle_validate, this, ::_1));
        endpoint.set_message_handler(bind(&websocket_lobby_server::handle_message, this, ::_1, ::_2));
        endpoint.set_http_handler(bind(&websocket_lobby_server::handle_http, this, ::_1));

        endpoint.init_asio();
        endpoint.listen(port);
        endpoint.start_accept();
        endpoint.run();
    } catch (websocketpp::exception const & e) {
        spdlog::get("game")->error("server exception {}", e.what());
        lobby.is_alive = false;
        std::exit(EXIT_FAILURE);
        return 1;
    } catch (...) {
        spdlog::get("game")->error("unknown server exception");
        lobby.is_alive = false;
        std::exit(EXIT_FAILURE);
        return 1;
    }

    return 0;
}

void websocket_lobby_server::handle_open(
    websocketpp::connection_hdl hdl
) {}

void websocket_lobby_server::handle_close(
    websocketpp::connection_hdl hdl
) {}

void websocket_lobby_server::handle_fail(
    websocketpp::connection_hdl hdl
) {}

bool websocket_lobby_server::handle_ping(
    websocketpp::connection_hdl hdl,
    std::string str
) {
    return true;
}

bool websocket_lobby_server::handle_pong(
    websocketpp::connection_hdl hdl,
    std::string str
) {
    return true;
}

void websocket_lobby_server::handle_pong_timeout(
    websocketpp::connection_hdl hdl,
    std::string str
) {}

void websocket_lobby_server::handle_interrupt(
    websocketpp::connection_hdl hdl
) {}

bool websocket_lobby_server::handle_validate(
    websocketpp::connection_hdl hdl
) {
    // todo
    // validate if connection is valid/allowed
    
    return true;
}

void websocket_lobby_server::handle_message(
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg
) {
    try {
        nlohmann::json j = nlohmann::json::parse(msg->get_payload());
        // std::cout << j.dump() << std::endl;

        if(j.at("request").get<std::string>() == "games") {
            return on_request_games(hdl, msg);
        }

    } catch(...) {
        try_send(&endpoint, hdl, websocketpp::frame::opcode::TEXT, {
            {"error", "json_parse_error"}
        });
    }
}

void websocket_lobby_server::handle_http(
    websocketpp::connection_hdl hdl
) {
    auto con = endpoint.get_con_from_hdl(hdl);

    const lobby_server& lobby = lobby_server::get_instance();

	con->set_status(websocketpp::http::status_code::ok);
	// con->set_header("Content-Type", "application/json"); // todo - we need to add this
	nlohmann::json j = lobby_event(lobby_event_games(lobby.get_games()));
	con->set_body(j.dump());
}

void websocket_lobby_server::on_request_games(
    websocketpp::connection_hdl hdl,
    websocketpp_server::message_ptr msg
) {
    const lobby_server& lobby = lobby_server::get_instance();

    try_send(&endpoint, hdl, websocketpp::frame::opcode::TEXT,
        lobby_event(lobby_event_games(lobby.get_games()))
    );
}

