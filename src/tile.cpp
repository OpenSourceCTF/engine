#include "tile.hpp"

void to_json(nlohmann::json& j, const tile& p)
{
    j = nlohmann::json{{"poly", p.poly}, {"col", p.col}, {"type", to_string(p.type)}};
}

void from_json(const nlohmann::json& j, tile& p)
{
    p.poly = j.at("poly").get<polygon>();
    p.col  = j.at("col").get<color>();
    p.type = tile_type_from_string(j.at("type").get<std::string>());
}
