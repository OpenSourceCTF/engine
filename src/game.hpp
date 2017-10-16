#ifndef ML_GAME_HPP
#define ML_GAME_HPP

#include <cstdint>
#include <chrono>
#include <thread>
#include "libs/json.hpp"
#include "map.hpp"

struct game
{
    std::uint16_t port;
    map* m;
    std::uint32_t max_points;
    std::uint32_t max_length;
    b2World * world;

    game(){}
    game(const std::uint16_t port, map* m);

    void run_physics();
};

void to_json(nlohmann::json& j, const game& p);

#endif

