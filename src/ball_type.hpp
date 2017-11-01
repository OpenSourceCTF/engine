#ifndef ML_BALL_TYPE_HPP
#define ML_BALL_TYPE_HPP

#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

enum class ball_type
{
    red, blue
};

std::string to_string(const ball_type m);
ball_type ball_type_from_string(const std::string & m);

#endif

