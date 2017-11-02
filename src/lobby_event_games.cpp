#include "lobby_event_games.hpp"

void to_json(nlohmann::json& j, const lobby_event_games_game& p)
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

void to_json(nlohmann::json& j, const lobby_event_games& p)
{
    j = nlohmann::json{{"games", p.games}};
}

