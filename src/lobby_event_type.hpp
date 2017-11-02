#ifndef ML_LOBBY_EVENT_TYPE_HPP
#define ML_LOBBY_EVENT_TYPE_HPP

#include <spdlog/spdlog.h>
#include <json.hpp>
#include <string>
#include <iostream>

enum class lobby_event_type
{
    games
};

std::string to_string(const lobby_event_type m);

#endif

