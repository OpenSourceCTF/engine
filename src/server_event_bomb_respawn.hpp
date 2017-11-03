#ifndef ML_SERVER_EVENT_BOMB_RESPAWN_HPP
#define ML_SERVER_EVENT_BOMB_RESPAWN_HPP

#include "bomb.hpp"

struct server_event_bomb_respawn
{
    bomb* m;

    server_event_bomb_respawn(
        bomb* m
    )
    : m(m)
    {}
};

#endif
