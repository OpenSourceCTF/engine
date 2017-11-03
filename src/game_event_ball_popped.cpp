#include "game_event_ball_popped.hpp"

void to_json(nlohmann::json& j, const game_event_ball_popped& p)
{
    j = nlohmann::json{
        {"id", p.id},
        {"px", p.pos.x},
        {"py", p.pos.y}
    };
}
