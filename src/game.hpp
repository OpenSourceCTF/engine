#ifndef ML_GAME_HPP
#define ML_GAME_HPP

#include <cstdint>
#include <chrono>
#include "libs/json.hpp"
#include "map.hpp"

struct game
{
    std::uint16_t port;
    map* m;
    std::uint32_t max_points;
    std::uint32_t max_length;

    game(){}
    game(
        const std::uint16_t port,
        map* m
    )
    : port(port)
    , m(m)
    , max_points(3)
    , max_length(15*60)
    {}
};

void to_json(nlohmann::json& j, const game& p);

#endif

