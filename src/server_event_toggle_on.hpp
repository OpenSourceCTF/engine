#ifndef ML_SERVER_EVENT_TOGGLE_ON_HPP
#define ML_SERVER_EVENT_TOGGLE_ON_HPP

#include "ball.hpp"
#include "toggle.hpp"

struct server_event_toggle_on
{
    ball* m_ball;
    toggle* m_toggle;

    server_event_toggle_on(
        ball* m_ball,
        toggle* m_toggle
    );
};

#endif
