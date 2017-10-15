#include "bomb.hpp"

void bomb::add_to_world(b2World * world)
{
    const settings& config = settings::get_instance();

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(this->x, this->y);

    body = world->CreateBody(&bdef);

    b2CircleShape bshape;
    bshape.m_p.Set(0.0f, 0.0f);
    bshape.m_radius = config.BOMB_RADIUS;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    body->CreateFixture(&fdef);
    body->SetUserData(static_cast<void*>(new collision_user_data(static_cast<void*>(this), collision_user_data_type::bomb)));
}

void bomb::explode()
{
    std::cout << "boom: " << x << "," << y << std::endl;
}

void to_json(nlohmann::json& j, const bomb& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}};
}

void from_json(const nlohmann::json& j, bomb& p)
{
    p.x = j.at("x").get<float>();
    p.y = j.at("y").get<float>();
}
