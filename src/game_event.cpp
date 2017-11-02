#include "game_event.hpp"

game_event::game_event(const game_event_gamesync e)
: type(game_event_type::gamesync)
, ptr(new game_event_gamesync(e))
{}

game_event::game_event(const game_event_player_joined e)
: type(game_event_type::player_joined)
, ptr(new game_event_player_joined(e))
{}

game_event::game_event(const game_event_chat e)
: type(game_event_type::chat)
, ptr(new game_event_chat(e))
{}

game_event::game_event(const game_event_honk e)
: type(game_event_type::honk)
, ptr(new game_event_honk(e))
{}

void to_json(nlohmann::json& j, const game_event& p)
{
    nlohmann::json data;

    switch(p.type) {
        case game_event_type::gamesync:
            to_json(data, *static_cast<game_event_gamesync*>(p.ptr));
            break;
        case game_event_type::player_joined:
            to_json(data, *static_cast<game_event_player_joined*>(p.ptr));
            break;
        case game_event_type::chat:
            to_json(data, *static_cast<game_event_chat*>(p.ptr));
            break;
        case game_event_type::honk:
            to_json(data, *static_cast<game_event_honk*>(p.ptr));
            break;
        default:
            spdlog::get("game")->error("game_event_type not enumerated in to_json");
            break;
    }

    j = nlohmann::json{
        {"event", to_string(p.type)},
        {"data", data}
    };
}

