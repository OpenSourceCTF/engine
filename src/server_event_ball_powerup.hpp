#ifndef ML_SERVER_EVENT_BALL_POWERUP_HPP
#define ML_SERVER_EVENT_BALL_POWERUP_HPP

#include "ball.hpp"
#include "powerup.hpp"

struct server_event_ball_powerup
{
    ball* m_ball;
    powerup* m_powerup;

    server_event_ball_powerup(
        ball* m_ball,
        powerup* m_powerup
    );
};

#endif

