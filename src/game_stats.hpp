#ifndef ML_GAME_STATS_HPP
#define ML_GAME_STATS_HPP

#include <cstdint>
#include <vector>

// todo
// maybe add methods to add these
// do we need player_stats at all?
struct game_stats
{
    std::vector<std::pair<std::string, std::string>> player_tags;
    std::vector<std::pair<std::string, std::string>> player_returns;
    std::vector<std::pair<std::string, ball_type>> caps;
    std::vector<std::pair<std::string, std::size_t>> player_joined;
    std::vector<std::pair<std::string, std::size_t>> player_left;

    game_stats()
    {}
};

#endif


