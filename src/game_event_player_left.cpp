#include "game_event_player_left.hpp"

void to_json(nlohmann::json& j, const game_event_player_left& p)
{
    j = nlohmann::json{
        {"player_id", p.p->player_id}
    };
}
