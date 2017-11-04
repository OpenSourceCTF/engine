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
    ballsync
};

std::string to_string(const server_event_type m);

#endif

