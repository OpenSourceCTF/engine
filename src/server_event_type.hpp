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
    ball_boosted,
    ball_score,
    ball_portal,
    ball_powerup,
    booster_respawn,
    powerup_respawn,
    portal_respawn,
    bomb_respawn,
    bomb_explosion,
    flag_grabbed,
    toggle_on,
    toggle_off,


    flag_transferred // neutral flag
};

std::string to_string(const server_event_type m);

#endif

