#ifndef ML_SERVER_EVENT_FLAG_GRABBED_HPP
#define ML_SERVER_EVENT_FLAG_GRABBED_HPP

#include "ball.hpp"
#include "flag.hpp"

struct server_event_flag_grabbed
{
    ball* m_ball;
    flag* m_flag;

    server_event_flag_grabbed(
        ball* m_ball,
        flag* m_flag
    );
};

#endif
