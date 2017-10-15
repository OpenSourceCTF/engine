#ifndef ML_POLYGON_HPP
#define ML_POLYGON_HPP

#include <array>
#include <Box2D/Box2D.h>
#include "libs/json.hpp"

struct polygon
{
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;

    polygon() : x1(0), y1(0), x2(0), y2(0), x3(0), y3(0) {}

    polygon(
        const float x1,
        const float y1,
        const float x2,
        const float y2,
        const float x3,
        const float y3
    )
    : x1(x1)
    , y1(y1)
    , x2(x2)
    , y2(y2)
    , x3(x3)
    , y3(y3)
    {
        const b2Vec2 center = get_center();

        std::array<b2Vec2, 3> v = {
            b2Vec2(x1, y1),
            b2Vec2(x2, y2),
            b2Vec2(x3, y3)
        };

        // https://stackoverflow.com/a/6989383 (reversed)
        auto clock = [&](const b2Vec2 a, const b2Vec2 b) {
			if (a.x - center.x >= 0 && b.x - center.x < 0) {
				return false;
			}

			if (a.x - center.x < 0 && b.x - center.x >= 0) {
				return true;
            }

			if (a.x - center.x == 0 && b.x - center.x == 0) {
				if (a.y - center.y >= 0 || b.y - center.y >= 0) {
					return a.y < b.y;
                }

				return b.y < a.y;
			}

			// compute the cross product of vectors (center -> a) x (center -> b)
			const int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
			if (det > 0) return true;
			if (det < 0) return false;

			// points a and b are on the same line from the center
			// check which point is closer to the center
			const int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
			const int d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
			return d1 < d2;
        };

        std::sort(std::begin(v), std::end(v), clock);
        this->x1 = v[0].x;
        this->y1 = v[0].y;
        this->x2 = v[1].x;
        this->y2 = v[1].y;
        this->x3 = v[2].x;
        this->y3 = v[2].y;
    }

    b2Vec2 get_center();
    std::array<b2Vec2, 3> get_vertices();
};

void to_json(nlohmann::json& j, const polygon& p);
void from_json(const nlohmann::json& j, polygon& p);

#endif

