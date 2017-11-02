#ifndef ML_CLIENT_ACTION_HPP
#define ML_CLIENT_ACTION_HPP

#include "client_action_type.hpp"
#include "client_action_movement.hpp"
#include "client_action_chat.hpp"

// todo make client_action types akin to game_event
struct client_action
{
    client_action_type type;
    void* ptr;

    client_action(client_action_chat e)
    : type(client_action_type::chat)
    , ptr(new client_action_chat(e))
    {}
};

#endif

