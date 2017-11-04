#include "server_event_teamchat.hpp"

server_event_teamchat::server_event_teamchat(
    player* p,
    std::string msg
)
: p(p)
, msg(msg)
{}
