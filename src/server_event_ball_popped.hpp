#ifndef ML_SERVER_EVENT_BALL_POPPED_HPP
#define ML_SERVER_EVENT_BALL_POPPED_HPP

#include "ball.hpp"

struct server_event_ball_popped
{
    ball* m;

    server_event_ball_popped(ball* m);
};

#endif
