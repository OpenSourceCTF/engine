#ifndef ML_GAME_EVENT_BALL_RB_EXPLODE_HPP
#define ML_GAME_EVENT_BALL_RB_EXPLODE_HPP

#include <cstdint>
#include <json/json_fwd.hpp>

struct game_event_ball_rb_explode
{
    const std::size_t id;

    game_event_ball_rb_explode(
        const std::size_t id
    );
};

void to_json(nlohmann::json& j, const game_event_ball_rb_explode& p);

#endif

