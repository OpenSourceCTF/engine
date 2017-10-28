#include "request_game_sync_response.hpp"

void to_json(nlohmann::json& j, const request_game_sync_response& p)
{
    std::vector<nlohmann::json> balls;
    balls.reserve(p.g.m->balls.size());

    for(auto & o : p.g.m->balls) {
        const b2Vec2 pos  = o->get_position();
        const b2Vec2 velo = o->get_linear_velocity();

        balls.emplace_back(nlohmann::json{
            {"type", to_string(o->type)},
            {"name", o->name},
            {"is_registered", o->is_registered},
            {"user_id", o->user_id},
            {"degree", o->degree},
            {"is_alive", o->is_alive},
            {"px", pos.x},  {"py", pos.y},
            {"vx", velo.x}, {"vy", velo.y},
        });
    }

    j = nlohmann::json{
        {"game", 
            {"timestep", p.g.timestep}
        },
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

