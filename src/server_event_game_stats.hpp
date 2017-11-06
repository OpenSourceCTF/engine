#ifndef ML_SERVER_EVENT_GAME_STATS_HPP
#define ML_SERVER_EVENT_GAME_STATS_HPP

#include <string>
#include "game.hpp"

struct game;

struct server_event_game_stats
{
    player* p;

    server_event_game_stats(
        player* p
    );
};

#endif

