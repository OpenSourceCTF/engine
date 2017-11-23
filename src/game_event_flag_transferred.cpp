#include "game_event_flag_transferred.hpp"
#include <json/json.hpp>

game_event_flag_transferred::game_event_flag_transferred(
    const std::size_t ball_send_id,
    const std::size_t ball_recv_id,
    const std::size_t flag_id
)
: ball_send_id(ball_send_id)
, ball_recv_id(ball_recv_id)
, flag_id(flag_id)
{}

void to_json(nlohmann::json& j, const game_event_flag_transferred& p)
{
    j = nlohmann::json{
        {"ball_send_id", p.ball_send_id},
        {"ball_recv_id", p.ball_recv_id},
        {"flag_id", p.flag_id}
    };
}
