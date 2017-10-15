#ifndef ML_SPIKE_HPP
#define ML_SPIKE_HPP

#include "libs/json.hpp"

struct spike
{
    float x;
    float y;

    spike(){}
    spike(
        const float x,
        const float y
    )
    : x(x)
    , y(y)
    {}
};

void to_json(nlohmann::json& j, const spike& p);
void from_json(const nlohmann::json& j, spike& p);


#endif

