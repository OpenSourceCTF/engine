#include "polygon.hpp"

b2Vec2 polygon::get_center()
{
    return b2Vec2((x1 + x2 + x3) / 3.0f, (y1 + y2 + y3) / 3.0f);
}

std::array<b2Vec2, 3> polygon::get_vertices()
{
    const b2Vec2 center = get_center();
    return {
        b2Vec2(x1 - center.x, y1 - center.y),
        b2Vec2(x2 - center.x, y2 - center.y),
        b2Vec2(x3 - center.x, y3 - center.y)
    };
}

void to_json(nlohmann::json& j, const polygon& p)
{
    j = nlohmann::json{
        {"x1", p.x1}, {"y1", p.y1},
        {"x2", p.x2}, {"y2", p.y2},
        {"x3", p.x3}, {"y3", p.y3}
    };
}

void from_json(const nlohmann::json& j, polygon& p)
{
    p.x1 = j.at("x1").get<float>() - 0.5f;
    p.y1 = j.at("y1").get<float>() - 0.5f;
    p.x2 = j.at("x2").get<float>() - 0.5f;
    p.y2 = j.at("y2").get<float>() - 0.5f;
    p.x3 = j.at("x3").get<float>() - 0.5f;
    p.y3 = j.at("y3").get<float>() - 0.5f;
}
