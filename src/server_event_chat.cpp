#include "server_event_chat.hpp"

server_event_chat::server_event_chat(
    player* p,
    std::string msg
)
: p(p)
, msg(msg)
{}
