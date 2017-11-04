#ifndef ML_SERVER_EVENT_TEAMCHAT_HPP
#define ML_SERVER_EVENT_TEAMCHAT_HPP

#include <string>
#include "player.hpp"

struct player;

struct server_event_teamchat
{
    player* p;
    std::string msg;

    server_event_teamchat(
        player* p,
        std::string msg
    );
};

#endif

