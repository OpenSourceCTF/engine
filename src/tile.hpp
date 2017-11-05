#ifndef ML_TILE_HPP
#define ML_TILE_HPP

#include <string>
#include <iostream>

#include <json/json_fwd.hpp>
#include "polygon.hpp"
#include "color.hpp"
#include "tile_type.hpp"

struct tile
{
    polygon poly;
    color col;
    tile_type type;

    tile();
    tile(
        const polygon poly,
        const color col,
        const tile_type type
    );
};

void to_json(nlohmann::json& j, const tile& p);
void from_json(const nlohmann::json& j, tile& p);

#endif

