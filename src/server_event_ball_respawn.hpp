#ifndef ML_SERVER_EVENT_BALL_RESPAWN_HPP
#define ML_SERVER_EVENT_BALL_RESPAWN_HPP

#include "ball.hpp"

struct ball;

struct server_event_ball_respawn
{
    ball* m;

    server_event_ball_respawn(ball* m);
};

#endif
