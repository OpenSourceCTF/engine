#ifndef ML_POLYGON_HPP
#define ML_POLYGON_HPP

#include <array>
#include <Box2D/Box2D.h>
#include <json/json_fwd.hpp>

struct polygon
{
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;

    polygon();

    polygon(
        const float x1,
        const float y1,
        const float x2,
        const float y2,
        const float x3,
        const float y3
    );

    b2Vec2 get_center() const;
    std::array<b2Vec2, 3> get_vertices();
};

void to_json(nlohmann::json& j, const polygon& p);
void from_json(const nlohmann::json& j, polygon& p);

#endif

