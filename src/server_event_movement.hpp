#ifndef ML_SERVER_EVENT_MOVEMENT_HPP
#define ML_SERVER_EVENT_MOVEMENT_HPP

#include <string>
#include "player.hpp"

struct player;

struct server_event_movement
{
    player* p;
    int xdir;
    int ydir;

    server_event_movement(
        player* p,
        int xdir,
        int ydir
    )
    : p(p)
    , xdir(xdir)
    , ydir(ydir)
    {}
};

#endif

