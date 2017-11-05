#ifndef ML_GAME_EVENT_GAME_FINISHED_HPP
#define ML_GAME_EVENT_GAME_FINISHED_HPP

#include <string>
#include <json/json_fwd.hpp>

struct game_event_game_finished
{
    game_event_game_finished();
};

void to_json(nlohmann::json& j, const game_event_game_finished& p);

#endif
