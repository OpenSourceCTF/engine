#ifndef ML_SERVER_EVENT_PLAYER_LEFT_HPP
#define ML_SERVER_EVENT_PLAYER_LEFT_HPP

#include <string>
#include "player.hpp"

struct player;

struct server_event_player_left
{
    player* p;

    server_event_player_left(player* p);
};

#endif
