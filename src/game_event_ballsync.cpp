#include "game_event_ballsync.hpp"

void to_json(nlohmann::json& j, const game_event_ballsync& p)
{
    std::vector<nlohmann::json> players;
    players.reserve(p.g->players.size());

    for(auto & o : p.g->players) {
        const b2Vec2 pos  = o->b->body->GetPosition();
        const b2Vec2 velo = o->b->body->GetLinearVelocity();
        const float spin  = o->b->body->GetAngularVelocity();

        players.emplace_back(nlohmann::json{
            {"player_id", o->player_id},
            {"px", pos.x},
            {"py", pos.y},
            {"vx", velo.x},
            {"vy", velo.y},
            {"spin", spin}
        });
    }

    j = nlohmann::json{
        {"players", players}
    };
}

