#ifndef ML_GAME_EVENT_TYPE_HPP
#define ML_GAME_EVENT_TYPE_HPP

#include <spdlog/spdlog.h>
#include <json.hpp>
#include <string>
#include <iostream>

enum class game_event_type
{
    gamesync,
    ballsync,
    player_joined,
    player_left,
    chat,
    teamchat,
    honk,
    ball_respawn,
    ball_popped,
    ball_boosted,
    ball_score,
    booster_respawn,
    powerup_respawn,
    portal_respawn,
    bomb_respawn,
    bomb_explosion,
    flag_grabbed,
    flag_transferred, // neutral flag
    toggle_on,
    toggle_off
};

std::string to_string(const game_event_type m);

#endif

