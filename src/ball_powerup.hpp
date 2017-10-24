#ifndef ML_BALL_POWERUP_HPP
#define ML_BALL_POWERUP_HPP

#include <cstdint>
#include "powerup_type.hpp"


struct ball_powerup
{
    powerup_type type;
    std::size_t counter;

    ball_powerup(
        const powerup_type type,
        const std::size_t counter
    )
    : type(type)
    , counter(counter)
    {}
};

#endif

