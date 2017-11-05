#include "game_event_toggle_off.hpp"
#include <json/json.hpp>

game_event_toggle_off::game_event_toggle_off(
    const std::size_t ball_id,
    const std::size_t toggle_id
)
: ball_id(ball_id)
, toggle_id(toggle_id)
{}

void to_json(nlohmann::json& j, const game_event_toggle_off& p)
{
    j = nlohmann::json{
        {"ball_id", p.ball_id},
        {"toggle_id", p.toggle_id}
    };
}
