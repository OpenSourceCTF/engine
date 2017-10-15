#ifndef ML_SETTINGS_HPP
#define ML_SETTINGS_HPP

#include "libs/INIReader.h"
#include "color.hpp"

struct settings
{
    bool is_initialized = false;

    float BALL_MOVEMENT_SPEED;
    float BALL_DAMPING;
    float BALL_DENSITY;
    float BALL_RESTITUTION;
    float BALL_FRICTION;

    color COLOR_TILE;
    color COLOR_WALL;

    float GUI_INITIAL_WINDOW_WIDTH;
    float GUI_INITIAL_WINDOW_HEIGHT;
    float GUI_MOVEMENT_SPEED_DIV;


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

