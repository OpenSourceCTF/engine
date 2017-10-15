#ifndef ML_PORTAL_HPP
#define ML_PORTAL_HPP

#include <cstdint>
#include "libs/json.hpp"

struct portal
{
    float x;
    float y;

    bool has_cooldown;
    std::uint32_t cooldown;

    bool has_destination;
    float dx;
    float dy;

    portal() {}
    portal(
        const float x,
        const float y
    )
    : x(x)
    , y(y)
    , has_cooldown(false)
    , has_destination(false)
    {}

    void set_cooldown(const std::uint32_t x);
    void set_destination(const float x, const float y);
};

void to_json(nlohmann::json& j, const portal& p);
void from_json(const nlohmann::json& j, portal& p);

#endif

