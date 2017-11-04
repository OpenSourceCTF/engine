#include "spike.hpp"

spike::~spike()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
    }
}

void spike::add_to_world(b2World * world)
{
    const settings& config = settings::get_instance();

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(this->x, this->y);

    body = world->CreateBody(&bdef);

    b2CircleShape bshape;
    bshape.m_p.Set(0.0f, 0.0f);
    bshape.m_radius = config.SPIKE_RADIUS;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    body->CreateFixture(&fdef);
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(collision_user_data_type::spike, this));
    body->SetUserData(static_cast<void*>(col_data.get()));
}

void to_json(nlohmann::json& j, const spike& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}};
}

void from_json(const nlohmann::json& j, spike& p)
{
    p = spike(j.at("x").get<float>(), j.at("y").get<float>());
}
