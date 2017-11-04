#ifndef ML_BALL_FLAG_HPP
#define ML_BALL_FLAG_HPP

#include <cstdint>
#include "flag.hpp"

struct flag;

struct ball_flag
{
    flag* f;

    ball_flag(flag* f);
};

#endif

