#include "client_action_type.hpp"

std::string to_string(const client_action_type m)
{
    switch(m) {
        case client_action_type::player_joined: return "player_joined"; break;
        case client_action_type::chat:          return "chat";          break;
        case client_action_type::movement:      return "movement";      break;
        default:
            spdlog::get("game")->error("client_action_type not enumerated in to_string");
            return "";
    }
}

