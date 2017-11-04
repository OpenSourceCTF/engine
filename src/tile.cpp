#include "tile.hpp"

tile::tile(){}
tile::tile(
    const polygon poly,
    const color col,
    const tile_type type
)
: poly(poly)
, col(col)
, type(type)
{}

void to_json(nlohmann::json& j, const tile& p)
{
    j = nlohmann::json{{"poly", p.poly}, {"col", p.col}, {"type", to_string(p.type)}};
}

void from_json(const nlohmann::json& j, tile& p)
{
    p = tile(
        j.at("poly").get<polygon>(),
        j.at("col").get<color>(),
        tile_type_from_string(j.at("type").get<std::string>())
    );
}
