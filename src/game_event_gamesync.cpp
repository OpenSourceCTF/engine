#include "game_event_gamesync.hpp"

game_event_gamesync::game_event_gamesync(const game& g)
: g(g)
{}

void to_json(nlohmann::json& j, const game_event_gamesync& p)
{
    std::vector<nlohmann::json> players;
    players.reserve(p.g.players.size());

    for(auto & o : p.g.players) {
        players.emplace_back(nlohmann::json{
            {"name", o->name},
            {"is_registered", o->is_registered},
            {"player_id", o->player_id},
            {"degree", o->degree},
        });
    }

    j = nlohmann::json{
        {"game", {
            {"max_points", p.g.max_points},
            {"max_length", p.g.max_length},
            {"red_points", p.g.red_points},
            {"blue_points", p.g.blue_points},
            {"timestep", p.g.timestep}
        }},
        {"players", players}
    };

    // add these (and maybe more?)
    // portals state, respawn, alive
    // toggles state, respawn, alive
    // bombs respawn, is_alive
    // boosters respawn, is_alive
    // gates state
    // flags state
    // powerups state, respawn
    // balls position, velocity, flags, powerups
}

