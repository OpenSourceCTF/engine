#include "game_event.hpp"

game_event::game_event(const game_event_gamesync e)
: type(game_event_type::gamesync)
, ptr(new game_event_gamesync(e))
{}

game_event::game_event(const game_event_ballsync e)
: type(game_event_type::ballsync)
, ptr(new game_event_ballsync(e))
{}

game_event::game_event(const game_event_player_joined e)
: type(game_event_type::player_joined)
, ptr(new game_event_player_joined(e))
{}

game_event::game_event(const game_event_player_left e)
: type(game_event_type::player_left)
, ptr(new game_event_player_left(e))
{}

game_event::game_event(const game_event_chat e)
: type(game_event_type::chat)
, ptr(new game_event_chat(e))
{}

game_event::game_event(const game_event_teamchat e)
: type(game_event_type::teamchat)
, ptr(new game_event_teamchat(e))
{}

game_event::game_event(const game_event_honk e)
: type(game_event_type::honk)
, ptr(new game_event_honk(e))
{}

game_event::game_event(const game_event_ball_respawn e)
: type(game_event_type::ball_respawn)
, ptr(new game_event_ball_respawn(e))
{}

game_event::game_event(const game_event_bomb_respawn e)
: type(game_event_type::bomb_respawn)
, ptr(new game_event_bomb_respawn(e))
{}

game_event::game_event(const game_event_powerup_respawn e)
: type(game_event_type::powerup_respawn)
, ptr(new game_event_powerup_respawn(e))
{}

game_event::game_event(const game_event_booster_respawn e)
: type(game_event_type::booster_respawn)
, ptr(new game_event_booster_respawn(e))
{}

game_event::game_event(const game_event_portal_respawn e)
: type(game_event_type::portal_respawn)
, ptr(new game_event_portal_respawn(e))
{}

game_event::game_event(const game_event_ball_popped e)
: type(game_event_type::ball_popped)
, ptr(new game_event_ball_popped(e))
{}

void to_json(nlohmann::json& j, const game_event& p)
{
    nlohmann::json data;

    switch(p.type) {
        case game_event_type::gamesync:
            to_json(data, *static_cast<game_event_gamesync*>(p.ptr));
            break;
        case game_event_type::ballsync:
            to_json(data, *static_cast<game_event_ballsync*>(p.ptr));
            break;
        case game_event_type::player_joined:
            to_json(data, *static_cast<game_event_player_joined*>(p.ptr));
            break;
        case game_event_type::player_left:
            to_json(data, *static_cast<game_event_player_left*>(p.ptr));
            break;
        case game_event_type::chat:
            to_json(data, *static_cast<game_event_chat*>(p.ptr));
            break;
        case game_event_type::teamchat:
            to_json(data, *static_cast<game_event_teamchat*>(p.ptr));
            break;
        case game_event_type::honk:
            to_json(data, *static_cast<game_event_honk*>(p.ptr));
            break;
        case game_event_type::ball_respawn:
            to_json(data, *static_cast<game_event_ball_respawn*>(p.ptr));
            break;
        case game_event_type::bomb_respawn:
            to_json(data, *static_cast<game_event_bomb_respawn*>(p.ptr));
            break;
        case game_event_type::powerup_respawn:
            to_json(data, *static_cast<game_event_powerup_respawn*>(p.ptr));
            break;
        case game_event_type::booster_respawn:
            to_json(data, *static_cast<game_event_booster_respawn*>(p.ptr));
            break;
        case game_event_type::portal_respawn:
            to_json(data, *static_cast<game_event_portal_respawn*>(p.ptr));
            break;
        case game_event_type::ball_popped:
            to_json(data, *static_cast<game_event_ball_popped*>(p.ptr));
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

