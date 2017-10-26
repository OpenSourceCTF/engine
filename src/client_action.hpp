#ifndef ML_CLIENT_ACTION_HPP
#define ML_CLIENT_ACTION_HPP

#include "client_action_type.hpp"

// todo make client_action types akin to game_event
struct client_action
{
    client_action_type type;

    client_action(){}
    client_action(
        const client_action_type type
    )
    : type(type)
    {}
};

#endif

