#ifndef ML_GAME_EVENT_BALL_BOOSTED_HPP
#define ML_GAME_EVENT_BALL_BOOSTED_HPP

#include <cstdint>
#include <json.hpp>
#include <Box2D/Box2D.h>

struct game_event_ball_boosted
{
    const std::size_t ball_id;
    const std::size_t booster_id;

    game_event_ball_boosted(
        const std::size_t ball_id,
        const std::size_t booster_id
    );
};

void to_json(nlohmann::json& j, const game_event_ball_boosted& p);

#endif

