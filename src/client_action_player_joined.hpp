#ifndef ML_CLIENT_ACTION_PLAYER_JOINED_HPP
#define ML_CLIENT_ACTION_PLAYER_JOINED_HPP

#include <string>
#include "player.hpp"

struct player;

struct client_action_player_joined
{
    player* p;

    client_action_player_joined(
        player* p
    )
    : p(p)
    {}
};

#endif
