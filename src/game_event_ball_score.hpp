#ifndef ML_GAME_EVENT_BALL_SCORE_HPP
#define ML_GAME_EVENT_BALL_SCORE_HPP

#include <cstdint>
#include <json.hpp>

struct game_event_ball_score
{
    const std::size_t id;

    game_event_ball_score(
        const std::size_t id
    );
};

void to_json(nlohmann::json& j, const game_event_ball_score& p);

#endif

