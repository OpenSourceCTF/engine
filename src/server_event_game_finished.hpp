#ifndef ML_SERVER_EVENT_GAME_FINISHED_HPP
#define ML_SERVER_EVENT_GAME_FINISHED_HPP

#include <string>
#include "game.hpp"

struct game;

struct server_event_game_finished
{
    game* g;

    server_event_game_finished(
        game* g
    );
};

#endif

