#ifndef ML_WALL_HPP
#define ML_WALL_HPP

#include <memory>
#include <array>
#include <Box2D/Box2D.h>
#include <json/json_fwd.hpp>
#include "polygon.hpp"
#include "color.hpp"

struct wall
{
    polygon poly;

    wall();
    wall(const polygon poly);
};

void to_json(nlohmann::json& j, const wall& p);
void from_json(const nlohmann::json& j, wall& p);

#endif

