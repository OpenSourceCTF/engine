#ifndef ML_CLIENT_ACTION_TYPE_HPP
#define ML_CLIENT_ACTION_TYPE_HPP

#include <spdlog/spdlog.h>
#include <string>

enum class client_action_type
{
    player_joined,
    chat,
    movement,
    honk
};

std::string to_string(const client_action_type m);

#endif

