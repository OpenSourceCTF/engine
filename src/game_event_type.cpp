#include "game_event_type.hpp"

std::string to_string(const game_event_type m)
{
    switch(m) {
        case game_event_type::gamesync:      return "gamesync";      break;
        case game_event_type::ballsync:      return "ballsync";      break;
        case game_event_type::player_joined: return "player_joined"; break;
        case game_event_type::player_left:   return "player_left";   break;
        case game_event_type::chat:          return "chat";          break;
        case game_event_type::teamchat:      return "teamchat";      break;
        case game_event_type::honk:          return "honk";          break;
        default:
            spdlog::get("game")->error("game_event_type not enumerated in to_string");
            return "";
    }
}
