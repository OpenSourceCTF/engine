#include "game_event_ball_portal.hpp"
#include <json/json.hpp>

game_event_ball_portal::game_event_ball_portal(
    const std::size_t ball_id,
    const std::size_t portal_id
)
: ball_id(ball_id)
, portal_id(portal_id)
{}

void to_json(nlohmann::json& j, const game_event_ball_portal& p)
{
    j = nlohmann::json{
        {"ball_id", p.ball_id},
        {"portal_id", p.portal_id}
    };
}
