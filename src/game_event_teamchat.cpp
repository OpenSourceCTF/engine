#include "game_event_teamchat.hpp"

void to_json(nlohmann::json& j, const game_event_teamchat& p)
{
    j = nlohmann::json{
        {"player_id", p.p->player_id},
        {"msg", p.msg}
    };
}
