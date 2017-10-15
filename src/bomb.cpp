#include "bomb.hpp"

void bomb::explode()
{
    std::cout << "boom: " << x << "," << y << std::endl;
}

void to_json(nlohmann::json& j, const bomb& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}};
}

void from_json(const nlohmann::json& j, bomb& p)
{
    p.x = j.at("x").get<float>();
    p.y = j.at("y").get<float>();
}
