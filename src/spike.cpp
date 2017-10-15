#include "spike.hpp"


void to_json(nlohmann::json& j, const spike& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}};
}

void from_json(const nlohmann::json& j, spike& p)
{
    p.x = j.at("x").get<float>();
    p.y = j.at("y").get<float>();
}
