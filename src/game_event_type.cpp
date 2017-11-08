#include "game_event_type.hpp"
#include <json/json.hpp>

std::string to_string(const game_event_type m)
{
    switch(m) {
        case game_event_type::gamesync:         return "gamesync";         break;
        case game_event_type::ballsync:         return "ballsync";         break;
        case game_event_type::player_joined:    return "player_joined";    break;
        case game_event_type::player_left:      return "player_left";      break;
        case game_event_type::chat:             return "chat";             break;
        case game_event_type::teamchat:         return "teamchat";         break;
        case game_event_type::honk:             return "honk";             break;
        case game_event_type::ball_respawn:     return "ball_respawn";     break;
        case game_event_type::ball_popped:      return "ball_popped";      break;
        case game_event_type::ball_boosted:     return "ball_boosted";     break;
        case game_event_type::ball_score:       return "ball_score";       break;
        case game_event_type::ball_portal:      return "ball_portal";      break;
        case game_event_type::ball_powerup:     return "ball_powerup";     break;
        case game_event_type::ball_rb_explode:  return "ball_rb_explode";  break;
        case game_event_type::booster_respawn:  return "booster_respawn";  break;
        case game_event_type::portal_respawn:   return "portal_respawn";   break;
        case game_event_type::powerup_respawn:  return "powerup_respawn";  break;
        case game_event_type::bomb_respawn:     return "bomb_respawn";     break;
        case game_event_type::bomb_explosion:   return "bomb_explosion";   break;
        case game_event_type::flag_grabbed:     return "flag_grabbed";     break;
        // case game_event_type::flag_transferred: return "flag_transferred"; break;
        case game_event_type::toggle_on:        return "toggle_on";        break;
        case game_event_type::toggle_off:       return "toggle_off";       break;
        case game_event_type::game_finished:    return "game_finished";    break;
        case game_event_type::game_stats:       return "game_stats";       break;
        case game_event_type::vote_player:      return "vote_player";      break;
    }
}
