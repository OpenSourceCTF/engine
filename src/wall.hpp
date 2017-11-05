#ifndef ML_WALL_HPP
#define ML_WALL_HPP

#include <memory>
#include <array>
#include <Box2D/Box2D.h>
#include <json/json.hpp>
#include "collision_user_data.hpp"
#include "polygon.hpp"
#include "color.hpp"

struct collision_user_data;

struct wall
{
    polygon poly;
    color col;
    std::shared_ptr<collision_user_data> col_data;

    wall();
    wall(
        const polygon poly,
        const color col
    );
};

void to_json(nlohmann::json& j, const wall& p);
void from_json(const nlohmann::json& j, wall& p);

#endif

