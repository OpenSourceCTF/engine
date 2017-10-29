#include "request_game_sync_response.hpp"

void to_json(nlohmann::json& j, const request_game_sync_response& p)
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
        {"game", 
            {"timestep", p.g.timestep}
        },
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

