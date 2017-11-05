#include "game_event_ball_boosted.hpp"
#include <json/json.hpp>

game_event_ball_boosted::game_event_ball_boosted(
    const std::size_t ball_id,
    const std::size_t booster_id
)
: ball_id(ball_id)
, booster_id(booster_id)
{}

void to_json(nlohmann::json& j, const game_event_ball_boosted& p)
{
    j = nlohmann::json{
        {"ball_id", p.ball_id},
        {"booster_id", p.booster_id}
    };
}
