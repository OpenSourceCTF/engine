#include "lobby_server.hpp"

lobby_server& lobby_server::get_instance()
{
    const settings& config = settings::get_instance();
    static lobby_server instance;

    if(instance.is_initialized) {
        return instance;
    }

    instance.is_initialized = true;
    instance.is_alive = true;
    instance.games.reserve(config.SERVER_GAMES);

    return instance;
}

void lobby_server::start_server()
{
    const settings& config = settings::get_instance();

    std::thread srv_lobby_thread(
        start_lobby_server,
        std::ref(*this),
        config.SERVER_LOBBY_PORT
    );
    srv_lobby_thread.detach();

    for(std::size_t i=0; i<config.SERVER_GAMES; ++i) {
        const std::uint16_t port = config.SERVER_GAME_PORT_START + i;
        const std::string map_src = config.SERVER_MAPS[i % config.SERVER_MAPS.size()];

        games.emplace_back(new game(port));
        game* g = games.back().get();
        g->load_map(map_src);
        g->spawn_srv_thread();
        g->spawn_phys_thread();
    }
}

game& lobby_server::get_game_from_port(const std::uint16_t port) const
{
    const settings& config = settings::get_instance();
    return *(games[(port - config.SERVER_GAME_PORT_START)].get());
}

std::vector<lobby_event_games_game> lobby_server::get_games() const
{
    const lobby_server& lobby = lobby_server::get_instance();

    std::vector<lobby_event_games_game> games;
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

	return games;
}
