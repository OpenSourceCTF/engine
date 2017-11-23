#include "server_event_flag_transferred.hpp"

server_event_flag_transferred::server_event_flag_transferred(
    ball* m_ball_send,
    ball* m_ball_recv,
    flag* m_flag
)
: m_ball_send(m_ball_send)
, m_ball_recv(m_ball_recv)
, m_flag(m_flag)
{}
