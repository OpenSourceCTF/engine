#ifndef ML_SERVER_EVENT_BALL_RB_EXPLODE_HPP
#define ML_SERVER_EVENT_BALL_RB_EXPLODE_HPP

#include "ball.hpp"

struct server_event_ball_rb_explode
{
    ball* m;

    server_event_ball_rb_explode(
        ball* m
    );
};

#endif

