#include "booster.hpp"

void booster::add_to_world(b2World * world)
{
    const settings& config = settings::get_instance();

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(this->x, this->y);

    body = world->CreateBody(&bdef);

    b2CircleShape bshape;
    bshape.m_p.Set(0.0f, 0.0f);
    bshape.m_radius = config.BOOSTER_RADIUS;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    body->CreateFixture(&fdef);
    body->SetUserData(static_cast<void*>(new collision_user_data(static_cast<void*>(this), collision_user_data_type::booster)));
}

void booster::step_on(ball* m)
{
    std::cout << "booster stepped on" << std::endl;
    m->get_boosted();
}

void to_json(nlohmann::json& j, const booster& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}, {"type", to_string(p.type)}};
}

void from_json(const nlohmann::json& j, booster& p)
{
    p.x      = j.at("x").get<float>();
    p.y      = j.at("y").get<float>();
    p.type   = booster_type_from_string(j.at("type").get<std::string>());
}
