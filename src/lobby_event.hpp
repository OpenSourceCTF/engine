#ifndef ML_LOBBY_EVENT_HPP
#define ML_LOBBY_EVENT_HPP

#include <spdlog/spdlog.h>
#include <json/json_fwd.hpp>
#include "lobby_event_type.hpp"
#include "lobby_event_games.hpp"

struct lobby_event_games;

struct lobby_event
{
    lobby_event_type type;
    void* ptr;

    lobby_event(const lobby_event_games e);
};

void to_json(nlohmann::json& j, const lobby_event& p);

#endif

