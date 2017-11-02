#ifndef ML_CLIENT_ACTION_CHAT_HPP
#define ML_CLIENT_ACTION_CHAT_HPP

#include <string>
#include "player.hpp"

struct player;

struct client_action_chat
{
    player* p;
    std::string msg;

    client_action_chat(
        player* p,
        std::string msg
    )
    : p(p)
    , msg(msg)
    {}
};

#endif

