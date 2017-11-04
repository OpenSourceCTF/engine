#ifndef ML_SERVER_EVENT_BALL_PORTAL_HPP
#define ML_SERVER_EVENT_BALL_PORTAL_HPP

#include "ball.hpp"
#include "portal.hpp"

struct server_event_ball_portal
{
    ball* m_ball;
    portal* m_portal;

    server_event_ball_portal(
        ball* m_ball,
        portal* m_portal
    );
};

#endif

