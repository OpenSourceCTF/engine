#include "lobby_event.hpp"
#include <json/json.hpp>

lobby_event::lobby_event(const lobby_event_games e)
: type(lobby_event_type::games)
, ptr(new lobby_event_games(e))
{}

void to_json(nlohmann::json& j, const lobby_event& p)
{
    nlohmann::json data;

    switch(p.type) {
        case lobby_event_type::games:
            to_json(data, *static_cast<lobby_event_games*>(p.ptr));
            break;
    }

    j = nlohmann::json{
        {"event", to_string(p.type)},
        {"data", data}
    };
}

