#ifndef ML_SERVER_EVENT_HPP
#define ML_SERVER_EVENT_HPP

#include "server_event_type.hpp"
#include "server_event_player_joined.hpp"
#include "server_event_player_left.hpp"
#include "server_event_chat.hpp"
#include "server_event_teamchat.hpp"
#include "server_event_movement.hpp"
#include "server_event_honk.hpp"
#include "server_event_ballsync.hpp"

struct server_event
{
    server_event_type type;
    void* ptr;

    server_event(server_event_chat e)
    : type(server_event_type::chat)
    , ptr(new server_event_chat(e))
    {}

    server_event(server_event_teamchat e)
    : type(server_event_type::teamchat)
    , ptr(new server_event_teamchat(e))
    {}

    server_event(server_event_player_joined e)
    : type(server_event_type::player_joined)
    , ptr(new server_event_player_joined(e))
    {}

    server_event(server_event_player_left e)
    : type(server_event_type::player_left)
    , ptr(new server_event_player_left(e))
    {}

    server_event(server_event_movement e)
    : type(server_event_type::movement)
    , ptr(new server_event_movement(e))
    {}

    server_event(server_event_honk e)
    : type(server_event_type::honk)
    , ptr(new server_event_honk(e))
    {}

    server_event(server_event_ballsync e)
    : type(server_event_type::ballsync)
    , ptr(new server_event_ballsync(e))
    {}
};

#endif

