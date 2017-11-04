#ifndef ML_SERVER_EVENT_HONK_HPP
#define ML_SERVER_EVENT_HONK_HPP

#include "player.hpp"

struct player;

struct server_event_honk
{
    player* p;

    server_event_honk(player* p);
};

#endif

