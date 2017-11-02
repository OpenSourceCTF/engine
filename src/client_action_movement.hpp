#ifndef ML_CLIENT_ACTION_MOVEMENT_HPP
#define ML_CLIENT_ACTION_MOVEMENT_HPP

#include <string>
#include "player.hpp"

struct player;

struct client_action_movement
{
    player* p;
    int xdir;
    int ydir;

    client_action_movement(
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

