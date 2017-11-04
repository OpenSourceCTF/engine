#ifndef ML_GAME_EVENT_PLAYER_LEFT_HPP
#define ML_GAME_EVENT_PLAYER_LEFT_HPP

#include <string>
#include <json.hpp>
#include "player.hpp"

struct game_event_player_left
{
    player* p;

    game_event_player_left(
        player* p
    )
    : p(p)
    {}
};

void to_json(nlohmann::json& j, const game_event_player_left& p);

#endif

