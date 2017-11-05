#ifndef ML_GAME_EVENT_BALL_PORTAL_HPP
#define ML_GAME_EVENT_BALL_PORTAL_HPP

#include <cstdint>
#include <json/json_fwd.hpp>

struct game_event_ball_portal
{
    const std::size_t ball_id;
    const std::size_t portal_id;

    game_event_ball_portal(
        const std::size_t ball_id,
        const std::size_t portal_id
    );
};

void to_json(nlohmann::json& j, const game_event_ball_portal& p);

#endif

