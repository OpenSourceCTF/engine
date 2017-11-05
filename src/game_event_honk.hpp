#ifndef ML_GAME_EVENT_HONK_HPP
#define ML_GAME_EVENT_HONK_HPP

#include <string>
#include <json/json.hpp>
#include "player.hpp"

struct game_event_honk
{
    player* p;

    game_event_honk(
        player* p
    );
};

void to_json(nlohmann::json& j, const game_event_honk& p);

#endif
