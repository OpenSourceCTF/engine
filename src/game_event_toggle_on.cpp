#include "game_event_toggle_on.hpp"

game_event_toggle_on::game_event_toggle_on(
    const std::size_t ball_id,
    const std::size_t toggle_id
)
: ball_id(ball_id)
, toggle_id(toggle_id)
{}

void to_json(nlohmann::json& j, const game_event_toggle_on& p)
{
    j = nlohmann::json{
        {"ball_id", p.ball_id},
        {"toggle_id", p.toggle_id}
    };
}
