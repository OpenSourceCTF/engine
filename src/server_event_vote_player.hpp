#ifndef ML_SERVER_EVENT_VOTE_PLAYER_HPP
#define ML_SERVER_EVENT_VOTE_PLAYER_HPP

#include <string>
#include "player.hpp"

struct player;

struct server_event_vote_player
{
    player* p;
    player* vp; // voted player
    std::string reason;

    server_event_vote_player(
        player* p,
        player* vp,
        std::string reason
    );
};

#endif

