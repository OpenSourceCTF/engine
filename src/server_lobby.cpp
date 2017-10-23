#include "server_lobby.hpp"

server_lobby& server_lobby::get_instance()
{
    const settings& config = settings::get_instance();
    static server_lobby instance;

    if(instance.is_initialized) {
        return instance;
    }

    instance.is_initialized = true;
    instance.is_alive = true;
    instance.games.reserve(config.SERVER_GAMES);

    return instance;
}

void server_lobby::start_server()
{
    const settings& config = settings::get_instance();

    std::thread srv_lobby_thread(
        start_lobby_server,
        std::ref(*this),
        config.SERVER_LOBBY_PORT
    );
    srv_lobby_thread.detach();

    for(std::size_t i=0; i<config.SERVER_GAMES; ++i) {
        std::ifstream t("maps/Boombox.json");
        std::stringstream buf;
        buf << t.rdbuf();

        const std::uint16_t port = config.SERVER_GAME_PORT_START + i;
        map * m = new map(nlohmann::json::parse(buf.str()));
        games.emplace_back(game(port, m));

        std::thread srv_game_thread(start_game_server, port);
        srv_game_thread.detach();


        std::thread phys_game_thread(
            &game::run_physics,
            games.back()
        );
        phys_game_thread.detach();
    }
}

game& server_lobby::get_game_from_port(const std::uint16_t port)
{
    const settings& config = settings::get_instance();
    return games[(port - config.SERVER_GAME_PORT_START)];
}

void to_json(nlohmann::json& j, const server_lobby& p)
{
    j = nlohmann::json{{"games", p.games}};
}
