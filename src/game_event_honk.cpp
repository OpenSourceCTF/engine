#include "game_event_honk.hpp"

game_event_honk::game_event_honk(
    player* p
)
: p(p)
{}

void to_json(nlohmann::json& j, const game_event_honk& p)
{
    j = nlohmann::json{
        {"player_id", p.p->player_id}
    };
}
