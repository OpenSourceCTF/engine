#include "powerup.hpp"

void powerup::add_to_world(b2World * world)
{
    const settings& config = settings::get_instance();

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(this->x, this->y);

    body = world->CreateBody(&bdef);

    b2CircleShape bshape;
    bshape.m_p.Set(0.0f, 0.0f);
    bshape.m_radius = config.POWERUP_RADIUS;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    body->CreateFixture(&fdef);
    body->SetUserData(static_cast<void*>(col_data));
}

void powerup::step_on(ball* m)
{
    std::cout << "powerup stepped on" << std::endl;
}

void to_json(nlohmann::json& j, const powerup& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}};
}

void from_json(const nlohmann::json& j, powerup& p)
{
    p.x = j.at("x").get<float>();
    p.y = j.at("y").get<float>();
}
