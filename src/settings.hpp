#ifndef ML_SETTINGS_HPP
#define ML_SETTINGS_HPP

#include "libs/INIReader.h"
#include "color.hpp"

struct settings
{
    bool is_initialized = false;

    float BALL_MOVEMENT_SPEED;
    float BALL_JUKEJUICE_SPEED;
    float BALL_DAMPING;
    float BALL_DENSITY;
    float BALL_RESTITUTION;
    float BALL_FRICTION;
    float BALL_RADIUS;
    int   BALL_RESPAWN_TIME;

    float SPIKE_RADIUS;

    float BOMB_RADIUS;
    int   BOMB_RESPAWN_TIME;
    float BOMB_EXPLOSION_RADIUS;
    float BOMB_EXPLOSION_FORCE;

    float TOGGLE_RADIUS;

    float BOOSTER_RADIUS;
    int   BOOSTER_RESPAWN_TIME;
    float BOOSTER_FORCE;

    float POWERUP_RADIUS;
    int   POWERUP_RESPAWN_TIME;
    int   POWERUP_LASTING_TIME;

    float FLAG_RADIUS;

    float PORTAL_RADIUS;

    color COLOR_TILE;
    color COLOR_WALL;

    int GUI_INITIAL_WINDOW_WIDTH;
    int GUI_INITIAL_WINDOW_HEIGHT;
    float GUI_MOVEMENT_SPEED_DIV;

    std::uint16_t SERVER_LOBBY_PORT;
    int SERVER_GAMES;
    int SERVER_GAME_PORT_START;


    // use this to initialize
    static settings& get_instance();

    // prevent copies
    settings(settings const&) = delete;
    // prevent move which could invalidate state
    settings(settings&&) = delete;

private:
    // prevent client from creating new instances
    settings() {}

};

#endif

