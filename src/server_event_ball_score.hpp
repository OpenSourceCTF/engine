#ifndef ML_SERVER_EVENT_BALL_SCORE_HPP
#define ML_SERVER_EVENT_BALL_SCORE_HPP

#include "ball.hpp"

struct server_event_ball_score
{
    ball* m;

    server_event_ball_score(ball* m);
};

#endif
