#include "request_lobby_games_response.hpp"

void to_json(nlohmann::json& j, const request_lobby_games_response& p)
{
    j = nlohmann::json{
        {"port",         p.port},
        {"max_points",   p.max_points},
        {"max_length",   p.max_length},
        {"map_name",     p.map_name},
        {"map_author",   p.map_author},
        {"player_count", p.player_count}
    };
}

