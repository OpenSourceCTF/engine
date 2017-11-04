#ifndef ML_LOBBY_EVENT_GAMES_HPP
#define ML_LOBBY_EVENT_GAMES_HPP

#include <cstdint>
#include <string>
#include <json.hpp>

struct lobby_event_games_game
{
    std::uint16_t port;
    std::uint32_t max_points;
    std::uint32_t max_length;
    std::size_t   timestep;
    std::string   map_name;
    std::string   map_author;
    std::size_t   player_count;

    lobby_event_games_game(
        std::uint16_t port,
        std::uint32_t max_points,
        std::uint32_t max_length,
        std::size_t   timestep,
        std::string   map_name,
        std::string   map_author,
        std::size_t   player_count
    );
};

struct lobby_event_games
{
    std::vector<lobby_event_games_game> games;

    lobby_event_games(
        std::vector<lobby_event_games_game> games
    );
};

void to_json(nlohmann::json& j, const lobby_event_games_game& p);
void to_json(nlohmann::json& j, const lobby_event_games& p);

#endif

