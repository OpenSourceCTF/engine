#include "game_event_ball_rb_explode.hpp"
#include <json/json.hpp>

game_event_ball_rb_explode::game_event_ball_rb_explode(
    const std::size_t id
)
: id(id)
{}

void to_json(nlohmann::json& j, const game_event_ball_rb_explode& p)
{
    j = nlohmann::json{
        {"id", p.id}
    };
}
