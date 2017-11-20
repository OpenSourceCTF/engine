#ifndef ML_POLYGON_HPP
#define ML_POLYGON_HPP

#include <array>
#include <Box2D/Box2D.h>
#include <json/json_fwd.hpp>
#include "color.hpp"

struct polygon
{
    b2Vec2 v1;
    b2Vec2 v2;
    b2Vec2 v3;
    color c1;
    color c2;
    color c3;
    b2Vec2 uv1;
    b2Vec2 uv2;
    b2Vec2 uv3;

    polygon();

    polygon(
        const b2Vec2 v1,
        const b2Vec2 v2,
        const b2Vec2 v3,
        const color c1
    );

    polygon(
        const b2Vec2 v1,
        const b2Vec2 v2,
        const b2Vec2 v3,
        const color c1,
        const color c2,
        const color c3
    );

    polygon(
        const b2Vec2 v1,
        const b2Vec2 v2,
        const b2Vec2 v3,
        const color c1,
        const color c2,
        const color c3,
        const b2Vec2 uv1,
        const b2Vec2 uv2,
        const b2Vec2 uv3
    );

    void make_clockwise();
    b2Vec2 get_center() const;
    std::array<b2Vec2, 3> get_vertices();
};

void to_json(nlohmann::json& j, const polygon& p);
void from_json(const nlohmann::json& j, polygon& p);

#endif

