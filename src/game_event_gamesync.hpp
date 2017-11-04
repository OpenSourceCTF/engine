#ifndef ML_GAME_EVENT_GAMESYNC_HPP
#define ML_GAME_EVENT_GAMESYNC_HPP

#include <string>
#include <json.hpp>
#include "game.hpp"
#include "ball_type.hpp"

struct game;

struct game_event_gamesync
{
    const game& g;
    game_event_gamesync(const game& g)
    : g(g)
    {}
};

void to_json(nlohmann::json& j, const game_event_gamesync& p);

#endif
