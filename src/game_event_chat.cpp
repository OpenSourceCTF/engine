#include "game_event_chat.hpp"
#include <json/json.hpp>

game_event_chat::game_event_chat(
    player* p,
    std::string msg
)
: p(p)
, msg(msg)
{}

void to_json(nlohmann::json& j, const game_event_chat& p)
{
    j = nlohmann::json{
        {"player_id", p.p->player_id},
        {"msg", p.msg}
    };
}
