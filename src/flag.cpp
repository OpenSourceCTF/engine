#include "flag.hpp"


void to_json(nlohmann::json& j, const flag& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}, {"type", to_string(p.type)}};
}

void from_json(const nlohmann::json& j, flag& p)
{
    p.x    = j.at("x").get<float>();
    p.y    = j.at("y").get<float>();
    p.type = flag_type_from_string(j.at("type").get<std::string>());
}
