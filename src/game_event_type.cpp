#include "game_event_type.hpp"

std::string to_string(const game_event_type m)
{
    switch(m) {
        case game_event_type::player_joined: return "player_joined"; break;
        default:
            std::cerr << "error: game_event_type not enumerated in to_string" << std::endl;
            return "";
    }
}
