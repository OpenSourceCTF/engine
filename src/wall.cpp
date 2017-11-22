#include "wall.hpp"
#include <json/json.hpp>

wall::wall(){}
wall::wall(const polygon poly)
: poly(poly)
{}

void to_json(nlohmann::json& j, const wall& p)
{
    j = nlohmann::json{{"poly", p.poly}};
}

void from_json(const nlohmann::json& j, wall& p)
{
    p = wall(j.at("poly").get<polygon>());
}
