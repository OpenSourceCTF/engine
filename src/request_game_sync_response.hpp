#ifndef ML_REQUEST_GAME_SYNC_RESPONSE_HPP
#define ML_REQUEST_GAME_SYNC_RESPONSE_HPP

#include <cstdint>
#include <string>
#include "libs/json.hpp"
#include "game.hpp"
#include "ball_type.hpp"

struct request_game_sync_response
{
    const game& g;
    request_game_sync_response(const game& g)
    : g(g)
    {}
};

void to_json(nlohmann::json& j, const request_game_sync_response& p);

#endif

