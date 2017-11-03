#ifndef ML_GAME_EVENT_POWERUP_RESPAWN_HPP
#define ML_GAME_EVENT_POWERUP_RESPAWN_HPP

#include <cstdint>
#include <Box2D/Box2D.h>
#include "powerup_type.hpp"

struct game_event_powerup_respawn
{
    const std::size_t id;
    const b2Vec2 pos;
    const powerup_type type;

    game_event_powerup_respawn(
        const std::size_t id,
        const b2Vec2 pos,
        const powerup_type type
    )
    : id(id)
    , pos(pos)
    , type(type)
    {}
};

void to_json(nlohmann::json& j, const game_event_powerup_respawn& p);

#endif

