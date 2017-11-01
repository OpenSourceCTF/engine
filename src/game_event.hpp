#ifndef ML_GAME_EVENT_HPP
#define ML_GAME_EVENT_HPP

#include <spdlog/spdlog.h>
#include <json.hpp>
#include "game_event_type.hpp"
#include "game_event_player_joined.hpp"
#include "game_event_ball_respawn.hpp"
#include "game_event_ball_popped.hpp"
#include "game_event_ball_boosted.hpp"
#include "game_event_portal_ball.hpp"
#include "game_event_powerup_respawn.hpp"
#include "game_event_bomb_explosion.hpp"
#include "game_event_flag_grabbed.hpp"
#include "game_event_flag_dropped.hpp"
#include "game_event_flag_transferred.hpp"
#include "game_event_team_score.hpp"
#include "game_event_toggle_on.hpp"
#include "game_event_toggle_off.hpp"

// this is used for relaying game events over network
struct game_event
{
    game_event_type type;
    void* ptr;

    // todo -- fill these out for other types
    game_event(const game_event_player_joined e)
    : type(game_event_type::player_joined)
    , ptr(new game_event_player_joined(e))
    {}
};

void to_json(nlohmann::json& j, const game_event& p);

#endif
