#ifndef ML_SERVER_EVENT_PORTAL_RESPAWN_HPP
#define ML_SERVER_EVENT_PORTAL_RESPAWN_HPP

#include "portal.hpp"

struct portal;

struct server_event_portal_respawn
{
    portal* m;

    server_event_portal_respawn(portal* m);
};

#endif
