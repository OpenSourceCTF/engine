#ifndef ML_SERVER_EVENT_GAMESYNC_HPP
#define ML_SERVER_EVENT_GAMESYNC_HPP

#include <string>
#include "player.hpp"

struct player;

struct server_event_gamesync
{
    player* p;

    server_event_gamesync(
        player* p
    );
};

#endif

