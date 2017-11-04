#ifndef ML_SERVER_EVENT_BOMB_EXPLOSION_HPP
#define ML_SERVER_EVENT_BOMB_EXPLOSION_HPP

#include "bomb.hpp"

struct server_event_bomb_explosion
{
    bomb* m;

    server_event_bomb_explosion(bomb* m);
};

#endif
