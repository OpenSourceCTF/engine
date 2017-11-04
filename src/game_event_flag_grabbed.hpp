#ifndef ML_GAME_EVENT_FLAG_GRABBED_HPP
#define ML_GAME_EVENT_FLAG_GRABBED_HPP

#include <cstdint>
#include <json.hpp>

struct game_event_flag_grabbed
{
    const std::size_t ball_id;
    const std::size_t flag_id;

    game_event_flag_grabbed(
        const std::size_t ball_id,
        const std::size_t flag_id
    );
};

void to_json(nlohmann::json& j, const game_event_flag_grabbed& p);

#endif

