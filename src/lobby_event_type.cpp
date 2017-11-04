#include "lobby_event_type.hpp"

std::string to_string(const lobby_event_type m)
{
    switch(m) {
        case lobby_event_type::games: return "games"; break;
        default:
            spdlog::get("game")->error("lobby_event_type not enumerated in to_string");
            return "";
    }
}
