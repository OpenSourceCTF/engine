#include "lobby_event_type.hpp"

std::string to_string(const lobby_event_type m)
{
    switch(m) {
        case lobby_event_type::games: return "games"; break;
    }
}
