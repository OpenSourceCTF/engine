#ifndef ML_BOOST_HPP
#define ML_BOOST_HPP

#include <string>
#include <iostream>
#include "libs/json.hpp"
#include "boost_type.hpp"


struct boost
{
    float x;
    float y;
    boost_type type;

    boost(){}
    boost(
        const float x,
        const float y,
        const boost_type type
    )
    : x(x)
    , y(y)
    , type(type)
    {}
};

void to_json(nlohmann::json& j, const boost& p);
void from_json(const nlohmann::json& j, boost& p);

#endif

