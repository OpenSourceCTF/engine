#include "server_event_type.hpp"

std::string to_string(const server_event_type m)
{
    switch(m) {
        case server_event_type::player_joined:    return "player_joined";    break;
        case server_event_type::player_left:      return "player_left";      break;
        case server_event_type::chat:             return "chat";             break;
        case server_event_type::teamchat:         return "teamchat";         break;
        case server_event_type::movement:         return "movement";         break;
        case server_event_type::honk:             return "honk";             break;
        case server_event_type::ballsync:         return "ballsync";         break;
        case server_event_type::ball_respawn:     return "ball_respawn";     break;
        case server_event_type::ball_popped:      return "ball_popped";      break;
        case server_event_type::booster_respawn:  return "booster_respawn";  break;
        case server_event_type::powerup_respawn:  return "powerup_respawn";  break;
        case server_event_type::bomb_respawn:     return "bomb_respawn";     break;
        case server_event_type::bomb_explosion:   return "bomb_explosion";   break;
        case server_event_type::portal_respawn:   return "portal_respawn";   break;
        case server_event_type::flag_grabbed:     return "flag_grabbed";     break;
        case server_event_type::flag_dropped:     return "flag_dropped";     break;
        case server_event_type::flag_transferred: return "flag_transferred"; break;
        case server_event_type::team_score:       return "team_score";       break;
        case server_event_type::toggle_on:        return "toggle_on";        break;
        case server_event_type::toggle_off:       return "toggle_off";       break;

        default:
            spdlog::get("game")->error("server_event_type not enumerated in to_string");
            return "";
    }
}

