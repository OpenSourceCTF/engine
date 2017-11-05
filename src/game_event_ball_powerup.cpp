#include "game_event_ball_powerup.hpp"
#include <json/json.hpp>

game_event_ball_powerup::game_event_ball_powerup(
    const std::size_t ball_id,
    const std::size_t powerup_id
)
: ball_id(ball_id)
, powerup_id(powerup_id)
{}

void to_json(nlohmann::json& j, const game_event_ball_powerup& p)
{
    j = nlohmann::json{
        {"ball_id", p.ball_id},
        {"powerup_id", p.powerup_id}
    };
}
