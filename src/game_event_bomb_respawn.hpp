#ifndef ML_GAME_EVENT_BOMB_RESPAWN_HPP
#define ML_GAME_EVENT_BOMB_RESPAWN_HPP

#include <cstdint>
#include <json/json.hpp>
#include <Box2D/Box2D.h>

struct game_event_bomb_respawn
{
    const std::size_t id;
    const b2Vec2 pos;

    game_event_bomb_respawn(
        const std::size_t id,
        const b2Vec2 pos
    );
};

void to_json(nlohmann::json& j, const game_event_bomb_respawn& p);

#endif

