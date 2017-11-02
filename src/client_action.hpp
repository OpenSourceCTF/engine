#ifndef ML_CLIENT_ACTION_HPP
#define ML_CLIENT_ACTION_HPP

#include "client_action_type.hpp"
#include "client_action_player_joined.hpp"
#include "client_action_chat.hpp"
#include "client_action_movement.hpp"

struct client_action
{
    client_action_type type;
    void* ptr;

    client_action(client_action_chat e)
    : type(client_action_type::chat)
    , ptr(new client_action_chat(e))
    {}

    client_action(client_action_player_joined e)
    : type(client_action_type::player_joined)
    , ptr(new client_action_player_joined(e))
    {}

    client_action(client_action_movement e)
    : type(client_action_type::movement)
    , ptr(new client_action_movement(e))
    {}
};

#endif

