#include "game_event_gamesync.hpp"

game_event_gamesync::game_event_gamesync(const game& g)
: g(g)
{}

void to_json(nlohmann::json& j, const game_event_gamesync& p)
{
    std::vector<nlohmann::json> players;
    std::vector<nlohmann::json> portals;
    std::vector<nlohmann::json> toggles;
    std::vector<nlohmann::json> bombs;
    std::vector<nlohmann::json> powerups;
    std::vector<nlohmann::json> boosters;
    std::vector<nlohmann::json> gates;
    std::vector<nlohmann::json> flags;
    std::vector<nlohmann::json> balls;

    for(auto & o : p.g.players) {
        players.emplace_back(nlohmann::json{
            {"name", o->name},
            {"is_registered", o->is_registered},
            {"player_id", o->player_id},
            {"degree", o->degree},
        });
    }

    for(auto & o : p.g.m->portals) {
        portals.emplace_back(nlohmann::json{
            {"id", o->id},
            {"cooldown_counter", o->cooldown_counter},
            {"is_alive", o->is_alive},
        });
    }

    for(auto & o : p.g.m->toggles) {
        toggles.emplace_back(nlohmann::json{
            {"id", o->id}
        });
    }

    for(auto & o : p.g.m->bombs) {
        bombs.emplace_back(nlohmann::json{
            {"id", o->id},
            {"is_alive", o->is_alive}
        });
    }

    for(auto & o : p.g.m->powerups) {
        powerups.emplace_back(nlohmann::json{
            {"id", o->id},
            {"is_alive", o->is_alive},
            {"type", to_string(o->type)}
        });
    }

    for(auto & o : p.g.m->boosters) {
        boosters.emplace_back(nlohmann::json{
            {"id", o->id},
            {"is_alive", o->is_alive}
        });
    }

    for(auto & o : p.g.m->gates) {
        gates.emplace_back(nlohmann::json{
            {"id", o->id},
            {"current", to_string(o->current)}
        });
    }

    for(auto & o : p.g.m->flags) {
        flags.emplace_back(nlohmann::json{
            {"id", o->id},
            {"is_alive", o->is_alive}
        });
    }

    for(auto & o : p.g.m->balls) {
        std::vector<nlohmann::json> b_powerups;
        for(auto & j : o->powerups) {
            b_powerups.emplace_back(nlohmann::json{
                {"type", to_string(j.type)},
                {"counter", j.counter}
            });
        }

        std::vector<nlohmann::json> b_flags;
        for(auto & j : o->flags) {
            b_flags.emplace_back(nlohmann::json{
                {"id", j.f->id}
            });
        }

        balls.emplace_back(nlohmann::json{
            {"id", o->id},
            {"is_alive", o->is_alive},
            {"player_id", o->player_ptr->player_id},
            {"powerups", b_powerups},
            {"flags", b_flags}
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
        {"players", players},
        {"portals", portals},
        {"toggles", toggles},
        {"bombs", bombs},
        {"powerups", powerups},
        {"boosters", boosters},
        {"gates", gates},
        {"flags", flags},
        {"balls", balls}
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

