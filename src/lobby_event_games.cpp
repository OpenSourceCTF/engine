#include "lobby_event_games.hpp"
#include <json/json.hpp>

lobby_event_games_game::lobby_event_games_game(
    std::uint16_t port,
    std::uint32_t max_points,
    std::uint32_t max_length,
    std::size_t   timestep,
    std::string   map_name,
    std::string   map_author,
    std::size_t   player_count
)
: port(port)
, max_points(max_points)
, max_length(max_length)
, timestep(timestep)
, map_name(map_name)
, map_author(map_author)
, player_count(player_count)
{}

lobby_event_games::lobby_event_games(
    std::vector<lobby_event_games_game> games
)
: games(games)
{}

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

