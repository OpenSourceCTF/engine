#ifndef ML_BOOST_TYPE_HPP
#define ML_BOOST_TYPE_HPP

#include <string>
#include <iostream>

#include "libs/json.hpp"

enum class boost_type
{
    all, blue, red
};

std::string to_string(const boost_type m);
boost_type boost_type_from_string(const std::string & m);

#endif

