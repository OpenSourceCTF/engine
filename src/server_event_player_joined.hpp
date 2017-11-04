#ifndef ML_SERVER_EVENT_PLAYER_JOINED_HPP
#define ML_SERVER_EVENT_PLAYER_JOINED_HPP

#include <string>
#include "player.hpp"

struct player;

struct server_event_player_joined
{
    player* p;

    server_event_player_joined(player* p);
};

#endif
