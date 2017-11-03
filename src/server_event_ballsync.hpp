#ifndef ML_SERVER_EVENT_BALLSYNC_HPP
#define ML_SERVER_EVENT_BALLSYNC_HPP

#include <string>
#include <json.hpp>
#include "game.hpp"
#include "ball_type.hpp"

struct game;

struct server_event_ballsync
{
    game* g;
    server_event_ballsync(game* g)
    : g(g)
    {}
};

#endif
