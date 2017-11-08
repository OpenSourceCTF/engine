#include "game_event_vote_player.hpp"
#include <json/json.hpp>

game_event_vote_player::game_event_vote_player(
    player* p,
    player* vp,
    std::string reason
)
: p(p)
, vp(vp)
, reason(reason)
{}

void to_json(nlohmann::json& j, const game_event_vote_player& p)
{
    j = nlohmann::json{
        {"player_id", p.p->player_id},
        {"voted_player_id", p.vp->player_id},
        {"reason", p.reason},
    };
}
