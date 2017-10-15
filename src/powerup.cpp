#include "powerup.hpp"

void to_json(nlohmann::json& j, const powerup& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}};
}

void from_json(const nlohmann::json& j, powerup& p)
{
    p.x = j.at("x").get<float>();
    p.y = j.at("y").get<float>();
}
