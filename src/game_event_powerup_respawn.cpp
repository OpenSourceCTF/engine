#include "game_event_powerup_respawn.hpp"

game_event_powerup_respawn::game_event_powerup_respawn(
    const std::size_t id,
    const b2Vec2 pos,
    const powerup_type type
)
: id(id)
, pos(pos)
, type(type)
{}

void to_json(nlohmann::json& j, const game_event_powerup_respawn& p)
{
    j = nlohmann::json{
        {"id", p.id},
        {"px", p.pos.x},
        {"py", p.pos.y},
        {"type", to_string(p.type)}
    };
}
