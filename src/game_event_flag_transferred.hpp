#ifndef ML_GAME_EVENT_FLAG_TRANSFERRED_HPP
#define ML_GAME_EVENT_FLAG_TRANSFERRED_HPP

#include <cstdint>
#include <json/json_fwd.hpp>

struct game_event_flag_transferred
{
    const std::size_t ball_send_id;
    const std::size_t ball_recv_id;
    const std::size_t flag_id;

    game_event_flag_transferred(
        const std::size_t ball_send_id,
        const std::size_t ball_recv_id,
        const std::size_t flag_id
    );
};

void to_json(nlohmann::json& j, const game_event_flag_transferred& p);

#endif

