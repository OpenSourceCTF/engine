#include "game_event_flag_grabbed.hpp"

game_event_flag_grabbed::game_event_flag_grabbed(
    const std::size_t ball_id,
    const std::size_t flag_id
)
: ball_id(ball_id)
, flag_id(flag_id)
{}

void to_json(nlohmann::json& j, const game_event_flag_grabbed& p)
{
    j = nlohmann::json{
        {"ball_id", p.ball_id},
        {"flag_id", p.flag_id}
    };
}
