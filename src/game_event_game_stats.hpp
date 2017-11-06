#ifndef ML_GAME_EVENT_GAME_STATS_HPP
#define ML_GAME_EVENT_GAME_STATS_HPP

#include <string>
#include <json/json_fwd.hpp>
#include "game.hpp"

struct game;

struct game_event_game_stats
{
    game* g;
    game_event_game_stats(game* g);
};

void to_json(nlohmann::json& j, const game_event_game_stats& p);

#endif
