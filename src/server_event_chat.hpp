#ifndef ML_SERVER_EVENT_CHAT_HPP
#define ML_SERVER_EVENT_CHAT_HPP

#include <string>
#include "player.hpp"

struct player;

struct server_event_chat
{
    player* p;
    std::string msg;

    server_event_chat(
        player* p,
        std::string msg
    );
};

#endif

