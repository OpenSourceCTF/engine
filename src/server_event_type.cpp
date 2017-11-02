#include "server_event_type.hpp"

std::string to_string(const server_event_type m)
{
    switch(m) {
        case server_event_type::player_joined: return "player_joined"; break;
        case server_event_type::chat:          return "chat";          break;
        case server_event_type::movement:      return "movement";      break;
        case server_event_type::honk:          return "honk";          break;
        default:
            spdlog::get("game")->error("server_event_type not enumerated in to_string");
            return "";
    }
}

