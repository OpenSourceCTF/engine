#ifndef ML_SPAWN_HPP
#define ML_SPAWN_HPP

#include <string>
#include <iostream>
#include <json/json_fwd.hpp>
#include "spawn_type.hpp"

struct spawn
{
    float x;
    float y;
    float radius;
    std::uint32_t weight;
    spawn_type type;

    spawn();
    spawn(
        const float x,
        const float y,
        const float radius,
        const std::uint32_t weight,
        const spawn_type type
    );
};

void to_json(nlohmann::json& j, const spawn& p);
void from_json(const nlohmann::json& j, spawn& p);

#endif
