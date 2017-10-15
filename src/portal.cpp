#include "portal.hpp"


void portal::set_cooldown(const std::uint32_t x)
{
    has_cooldown = true;
    cooldown = x;
}

void portal::set_destination(const float x, const float y)
{
    has_destination = true;
    dx = x;
    dy = y;
}

void to_json(nlohmann::json& j, const portal& p)
{
    j = nlohmann::json{
        {"x", p.x}, {"y", p.y},
        {"has_cooldown", p.has_cooldown}, {"cooldown", p.cooldown},
        {"has_destination", p.has_destination}, {"dx", p.dx}, {"dy", p.dy}
    };
}

void from_json(const nlohmann::json& j, portal& p)
{
    p.x = j.at("x").get<float>();
    p.y = j.at("y").get<float>();

    p.has_cooldown = j.at("has_cooldown").get<bool>();
    if(p.has_cooldown) {
        p.cooldown = j.at("cooldown").get<int>();
    }

    p.has_destination = j.at("has_destination").get<bool>();
    if(p.has_destination) {
        p.dx = j.at("dx").get<float>();
        p.dy = j.at("dy").get<float>();
    }
}

