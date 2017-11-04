#include "wall.hpp"

wall::wall(){}
wall::wall(
    const polygon poly,
    const color col
)
: poly(poly)
, col(col)
, col_data(nullptr)
{}

void to_json(nlohmann::json& j, const wall& p)
{
    j = nlohmann::json{{"poly", p.poly}, {"col", p.col}};
}

void from_json(const nlohmann::json& j, wall& p)
{
    p = wall(
        j.at("poly").get<polygon>(),
        j.at("col").get<color>()
    );
}
