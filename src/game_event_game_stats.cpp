#include "game_event_game_stats.hpp"
#include <json/json.hpp>

game_event_game_stats::game_event_game_stats(game* g)
: g(g)
{}

void to_json(nlohmann::json& j, const game_event_game_stats& p)
{
    // todo
}
