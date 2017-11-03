#ifndef ML_SERVER_EVENT_TYPE_HPP
#define ML_SERVER_EVENT_TYPE_HPP

#include <spdlog/spdlog.h>
#include <string>

enum class server_event_type
{
    player_joined,
    player_left,
    chat,
    teamchat,
    movement,
    honk,
    ballsync,
    ball_respawn,
    ball_popped,
    booster_respawn,
    powerup_respawn,
    portal_respawn,
    bomb_respawn,
    bomb_explosion,
    flag_grabbed,
    flag_dropped,
    flag_transferred, // neutral flag
    team_score,
    toggle_on,
    toggle_off
};

std::string to_string(const server_event_type m);

#endif

