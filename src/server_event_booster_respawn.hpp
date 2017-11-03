#ifndef ML_SERVER_EVENT_BOOSTER_RESPAWN_HPP
#define ML_SERVER_EVENT_BOOSTER_RESPAWN_HPP

#include "booster.hpp"

struct server_event_booster_respawn
{
    booster* m;

    server_event_booster_respawn(
        booster* m
    )
    : m(m)
    {}
};

#endif
