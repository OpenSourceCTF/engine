#include "game_event_ballsync.hpp"
#include <json/json.hpp>

game_event_ballsync::game_event_ballsync(game* g)
: g(g)
{}

void to_json(nlohmann::json& j, const game_event_ballsync& p)
{
    std::vector<nlohmann::json> balls;
    balls.reserve(p.g->m->balls.size());

    for(auto & o : p.g->m->balls) {
        const b2Vec2 pos  = o->body->GetPosition();
        const b2Vec2 velo = o->body->GetLinearVelocity();
        const float spin  = o->body->GetAngularVelocity();

        balls.emplace_back(nlohmann::json{
            {"id", o->id},
            {"px", pos.x},
            {"py", pos.y},
            {"vx", velo.x},
            {"vy", velo.y},
            {"spin", spin}
        });
    }

    j = nlohmann::json{
        {"timestep", p.g->timestep},
        {"balls", balls}
    };
}

