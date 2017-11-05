#include "spawn.hpp"
#include <json/json.hpp>

spawn::spawn(){}
spawn::spawn(
    const float x,
    const float y,
    const float radius,
    const std::uint32_t weight,
    const spawn_type type
)
: x(x)
, y(y)
, radius(radius)
, weight(weight)
, type(type)
{}

void to_json(nlohmann::json& j, const spawn& p)
{
    j = nlohmann::json{
        {"x",      p.x},
        {"y",      p.y},
        {"radius", p.radius},
        {"weight", p.weight},
        {"type",   to_string(p.type)}
    };
}

void from_json(const nlohmann::json& j, spawn& p)
{
    p = spawn(
        j.at("x").get<float>(),
        j.at("y").get<float>(),
        j.at("radius").get<float>(),
        j.at("weight").get<float>(),
        spawn_type_from_string(j.at("type").get<std::string>())
    );
}

