#ifndef ML_CLIENT_ACTION_HONK_HPP
#define ML_CLIENT_ACTION_HONK_HPP

#include "player.hpp"

struct player;

struct client_action_honk
{
    player* p;

    client_action_honk(
        player* p
    )
    : p(p)
    {}
};

#endif

