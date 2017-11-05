#ifndef ML_GAME_EVENT_BALL_POWERUP_HPP
#define ML_GAME_EVENT_BALL_POWERUP_HPP

#include <cstdint>
#include <json/json.hpp>

struct game_event_ball_powerup
{
    const std::size_t ball_id;
    const std::size_t powerup_id;

    game_event_ball_powerup(
        const std::size_t ball_id,
        const std::size_t powerup_id
    );
};

void to_json(nlohmann::json& j, const game_event_ball_powerup& p);

#endif

