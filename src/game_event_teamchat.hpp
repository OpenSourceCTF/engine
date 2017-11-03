#ifndef ML_GAME_EVENT_TEAMCHAT_HPP
#define ML_GAME_EVENT_TEAMCHAT_HPP

#include <string>
#include <json.hpp>
#include "player.hpp"

struct game_event_teamchat
{
    player* p;
    std::string msg;

    game_event_teamchat(
        player* p,
        std::string msg
    )
    : p(p)
    , msg(msg)
    {}
};

void to_json(nlohmann::json& j, const game_event_teamchat& p);

#endif
