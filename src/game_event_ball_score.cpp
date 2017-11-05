#include "game_event_ball_score.hpp"
#include <json/json.hpp>

game_event_ball_score::game_event_ball_score(
    const std::size_t id
)
: id(id)
{}

void to_json(nlohmann::json& j, const game_event_ball_score& p)
{
    j = nlohmann::json{
        {"id", p.id}
    };
}
