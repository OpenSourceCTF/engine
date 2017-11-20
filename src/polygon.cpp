#include "polygon.hpp"
#include <json/json.hpp>

polygon::polygon()
: v1(b2Vec2(0, 0))
, v2(b2Vec2(0, 0))
, v3(b2Vec2(0, 0))
, c1(color(0, 0, 0, 0))
, c2(color(0, 0, 0, 0))
, c3(color(0, 0, 0, 0))
, uv1(b2Vec2(0, 0))
, uv2(b2Vec2(0, 0))
, uv3(b2Vec2(0, 0))
{}

polygon::polygon(
    const b2Vec2 v1,
    const b2Vec2 v2,
    const b2Vec2 v3,
    const color c
)
: v1(v1)
, v2(v2)
, v3(v3)
, c1(c)
, c2(c)
, c3(c)
, uv1(b2Vec2(0, 0))
, uv2(b2Vec2(0, 0))
, uv3(b2Vec2(0, 0))
{
    make_clockwise();
}

polygon::polygon(
    const b2Vec2 v1,
    const b2Vec2 v2,
    const b2Vec2 v3,
    const color c1,
    const color c2,
    const color c3
)
: v1(v1)
, v2(v2)
, v3(v3)
, c1(c1)
, c2(c2)
, c3(c3)
, uv1(b2Vec2(0, 0))
, uv2(b2Vec2(0, 0))
, uv3(b2Vec2(0, 0))
{
    make_clockwise();
}

polygon::polygon(
    const b2Vec2 v1,
    const b2Vec2 v2,
    const b2Vec2 v3,
    const color c1,
    const color c2,
    const color c3,
    const b2Vec2 uv1,
    const b2Vec2 uv2,
    const b2Vec2 uv3
)
: v1(v1)
, v2(v2)
, v3(v3)
, c1(c1)
, c2(c2)
, c3(c3)
, uv1(uv1)
, uv2(uv2)
, uv3(uv3)
{
    make_clockwise();
}

void polygon::make_clockwise()
{
    const b2Vec2 center = get_center();

    // we sort vertices by idx so we know how
    // to sort colors or whatever else per vertex 
    std::array<std::size_t, 3> v_idx = {0, 1, 2};

    // save original data
    std::array<b2Vec2, 3> v  = {v1, v2, v3};
    std::array<color,  3> c  = {c1, c2, c3};
    std::array<b2Vec2, 3> uv = {uv1, uv2, uv3};

    // https://stackoverflow.com/a/6989383 (reversed)
    auto clock = [&](const std::size_t a_idx, const std::size_t b_idx) {
        const b2Vec2 a = v[a_idx];
        const b2Vec2 b = v[b_idx];

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

    std::sort(std::begin(v_idx), std::end(v_idx), clock);

    this->v1 = v[v_idx[0]];
    this->v2 = v[v_idx[1]];
    this->v3 = v[v_idx[2]];

    this->c1 = c[v_idx[0]];
    this->c2 = c[v_idx[1]];
    this->c3 = c[v_idx[2]];

    this->uv1 = uv[v_idx[0]];
    this->uv2 = uv[v_idx[1]];
    this->uv3 = uv[v_idx[2]];
}

b2Vec2 polygon::get_center() const
{
    return b2Vec2((v1.x + v2.x + v3.x) / 3.0f, (v1.y + v2.y + v3.y) / 3.0f);
}

std::array<b2Vec2, 3> polygon::get_vertices()
{
    const b2Vec2 center = get_center();
    return {
        b2Vec2(v1.x - center.x, v1.y - center.y),
        b2Vec2(v2.x - center.x, v2.y - center.y),
        b2Vec2(v3.x - center.x, v3.y - center.y)
    };
}

void to_json(nlohmann::json& j, const polygon& p)
{
    j = nlohmann::json{
        {"x1", p.v1.x}, {"y1", p.v1.y},
        {"x2", p.v2.x}, {"y2", p.v2.y},
        {"x3", p.v3.x}, {"y3", p.v3.y},
        {"c1r", p.c1.r}, {"c1g", p.c1.g}, {"c1b", p.c1.b}, {"c1a", p.c1.b},
        {"c2r", p.c2.r}, {"c2g", p.c2.g}, {"c2b", p.c2.b}, {"c2a", p.c2.b},
        {"c3r", p.c3.r}, {"c3g", p.c3.g}, {"c3b", p.c3.b}, {"c3a", p.c3.b},
        {"u1", p.uv1.x}, {"v1", p.uv1.y},
        {"u2", p.uv2.x}, {"v2", p.uv2.y},
        {"u3", p.uv3.x}, {"v3", p.uv3.y},
    };
}

void from_json(const nlohmann::json& j, polygon& p)
{
    p = polygon(
        b2Vec2(j.at("x1").get<float>(), j.at("y1").get<float>()),
        b2Vec2(j.at("x2").get<float>(), j.at("y2").get<float>()),
        b2Vec2(j.at("x3").get<float>(), j.at("y3").get<float>()),
        color(
            j.at("c1r").get<int>(),
            j.at("c1g").get<int>(),
            j.at("c1b").get<int>(),
            j.at("c1a").get<int>()
        ),
        color(
            j.at("c2r").get<int>(),
            j.at("c2g").get<int>(),
            j.at("c2b").get<int>(),
            j.at("c2a").get<int>()
        ),
        color(
            j.at("c3r").get<int>(),
            j.at("c3g").get<int>(),
            j.at("c3b").get<int>(),
            j.at("c3a").get<int>()
        ),
        b2Vec2(j.at("u1").get<float>(), j.at("v1").get<float>()),
        b2Vec2(j.at("u2").get<float>(), j.at("v2").get<float>()),
        b2Vec2(j.at("u3").get<float>(), j.at("v3").get<float>())
    );
}
