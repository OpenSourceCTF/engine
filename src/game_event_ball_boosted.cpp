#include "game_event_ball_boosted.hpp"

void to_json(nlohmann::json& j, const game_event_ball_boosted& p)
{
    j = nlohmann::json{
        {"ball_id", p.ball_id},
        {"booster_id", p.booster_id}
    };
}
