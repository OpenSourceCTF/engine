#ifndef ML_GAME_EVENT_TYPE_HPP
#define ML_GAME_EVENT_TYPE_HPP

#include "libs/json.hpp"
#include <string>
#include <iostream>

enum class game_event_type
{
    player_joined,
    ball_respawn,
    ball_popped,
    powerup_respawn,
    bomb_explosion,
    flag_grabbed,
    flag_dropped,
    flag_transferred, // neutral flag
    team_score,
    toggle_on,
    toggle_off
};

std::string to_string(const game_event_type m);

#endif

