#include "spawn.hpp"

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

