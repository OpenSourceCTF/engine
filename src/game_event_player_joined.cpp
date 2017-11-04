#include "game_event_player_joined.hpp"

game_event_player_joined::game_event_player_joined(player* p)
: p(p)
{}

void to_json(nlohmann::json& j, const game_event_player_joined& p)
{
    j = nlohmann::json{
        {"player", {
            {"player_id", p.p->player_id},
            {"is_registered", p.p->is_registered},
            {"name", p.p->name},
            {"degree", p.p->degree},
            {"ball_id", p.p->b->id}
        }}
    };
}
