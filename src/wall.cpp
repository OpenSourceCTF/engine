#include "wall.hpp"

void wall::add_to_world(b2World * world)
{
    const b2Vec2 center = poly.get_center();

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(center.x, center.y);
    bdef.angle = 0.0f;

    body = world->CreateBody(&bdef);

    const std::array<b2Vec2, 3> vertices = poly.get_vertices();

    b2PolygonShape bshape;
    bshape.Set(vertices.data(), vertices.size());

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.density = 1;
    body->CreateFixture(&fdef);
    body->SetUserData(static_cast<void*>(new collision_user_data(static_cast<void*>(this), collision_user_data_type::wall)));
}

void to_json(nlohmann::json& j, const wall& p)
{
    j = nlohmann::json{{"poly", p.poly}, {"col", p.col}};
}

void from_json(const nlohmann::json& j, wall& p)
{
    p.poly = j.at("poly").get<polygon>();
    p.col = j.at("col").get<color>();
}
