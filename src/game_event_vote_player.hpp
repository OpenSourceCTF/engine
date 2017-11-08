#ifndef ML_GAME_EVENT_VOTE_PLAYER_HPP
#define ML_GAME_EVENT_VOTE_PLAYER_HPP

#include <string>
#include <json/json_fwd.hpp>
#include "player.hpp"

struct game_event_vote_player
{
    player* p;
    player* vp;
    std::string reason;

    game_event_vote_player(
        player* p,
        player* vp,
        std::string reason
    );
};

void to_json(nlohmann::json& j, const game_event_vote_player& p);

#endif
