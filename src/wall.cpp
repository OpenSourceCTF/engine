#include "wall.hpp"

void to_json(nlohmann::json& j, const wall& p)
{
    j = nlohmann::json{{"poly", p.poly}, {"col", p.col}};
}

void from_json(const nlohmann::json& j, wall& p)
{
    p.poly = j.at("poly").get<polygon>();
    p.col = j.at("col").get<color>();
}
