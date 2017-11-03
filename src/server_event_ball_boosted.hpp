#ifndef ML_SERVER_EVENT_BALL_BOOSTED_HPP
#define ML_SERVER_EVENT_BALL_BOOSTED_HPP

#include "ball.hpp"
#include "booster.hpp"

struct server_event_ball_boosted
{
    ball* m_ball;
    booster* m_booster;

    server_event_ball_boosted(
        ball* m_ball,
        booster* m_booster
    )
    : m_ball(m_ball)
    , m_booster(m_booster)
    {}
};

#endif
