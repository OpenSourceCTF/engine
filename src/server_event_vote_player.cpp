#include "server_event_vote_player.hpp"

server_event_vote_player::server_event_vote_player(
    player* p,
    player* vp,
    std::string reason
)
: p(p)
, vp(vp)
, reason(reason)
{}

