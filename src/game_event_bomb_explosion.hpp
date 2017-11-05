#ifndef ML_GAME_EVENT_BOMB_EXPLOSION_HPP
#define ML_GAME_EVENT_BOMB_EXPLOSION_HPP

#include <cstdint>
#include <json/json_fwd.hpp>
#include <Box2D/Box2D.h>

struct game_event_bomb_explosion
{
    const std::size_t id;

    game_event_bomb_explosion(
        const std::size_t id
    );
};

void to_json(nlohmann::json& j, const game_event_bomb_explosion& p);

#endif

