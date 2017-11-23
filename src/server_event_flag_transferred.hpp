#ifndef ML_SERVER_EVENT_FLAG_TRANSFERRED_HPP
#define ML_SERVER_EVENT_FLAG_TRANSFERRED_HPP

#include "ball.hpp"
#include "flag.hpp"

struct server_event_flag_transferred
{
    ball* m_ball_send;
    ball* m_ball_recv;
    flag* m_flag;

    server_event_flag_transferred(
        ball* m_ball_send,
        ball* m_ball_recv,
        flag* m_flag
    );
};

#endif

