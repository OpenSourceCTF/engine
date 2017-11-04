#include "game_event_bomb_explosion.hpp"

game_event_bomb_explosion::game_event_bomb_explosion(
    const std::size_t id
)
: id(id)
{}

void to_json(nlohmann::json& j, const game_event_bomb_explosion& p)
{
    j = nlohmann::json{
        {"id", p.id}
    };
}

