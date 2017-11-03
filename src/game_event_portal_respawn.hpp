#ifndef ML_GAME_EVENT_PORTAL_RESPAWN_HPP
#define ML_GAME_EVENT_PORTAL_RESPAWN_HPP

#include <cstdint>
#include <json.hpp>
#include <Box2D/Box2D.h>

struct game_event_portal_respawn
{
    const std::size_t id;
    const b2Vec2 pos;

    game_event_portal_respawn(
        const std::size_t id,
        const b2Vec2 pos
    )
    : id(id)
    , pos(pos)
    {}
};

void to_json(nlohmann::json& j, const game_event_portal_respawn& p);

#endif

