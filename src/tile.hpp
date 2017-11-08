#ifndef ML_TILE_HPP
#define ML_TILE_HPP

#include <string>
#include <iostream>
#include <Box2D/Box2D.h>

#include <json/json_fwd.hpp>
#include "polygon.hpp"
#include "color.hpp"
#include "tile_type.hpp"
#include "collision_user_data.hpp"
#include "game_accessor.hpp"

struct collision_user_data;

struct tile
{
    game_accessor game;
    polygon poly;
    color col;
    tile_type type;
    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;

    tile();
    tile(
        const polygon poly,
        const color col,
        const tile_type type
    );

    void add_to_world(b2World * world);
};

void to_json(nlohmann::json& j, const tile& p);
void from_json(const nlohmann::json& j, tile& p);

#endif

