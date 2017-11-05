#ifndef ML_GAME_EVENT_CHAT_HPP
#define ML_GAME_EVENT_CHAT_HPP

#include <string>
#include <json/json_fwd.hpp>
#include "player.hpp"

struct game_event_chat
{
    player* p;
    std::string msg;

    game_event_chat(
        player* p,
        std::string msg
    );
};

void to_json(nlohmann::json& j, const game_event_chat& p);

#endif

