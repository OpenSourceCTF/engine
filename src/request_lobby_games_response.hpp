#ifndef ML_REQUEST_LOBBY_GAMES_RESPONSE_HPP
#define ML_REQUEST_LOBBY_GAMES_RESPONSE_HPP

#include <cstdint>
#include <string>
#include "libs/json.hpp"

struct request_lobby_games_response
{
    std::uint16_t port;
    std::uint32_t max_points;
    std::uint32_t max_length;
    std::size_t   timestep;
    std::string   map_name;
    std::string   map_author;
    std::size_t   player_count;

    request_lobby_games_response(
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
};

void to_json(nlohmann::json& j, const request_lobby_games_response& p);

#endif

