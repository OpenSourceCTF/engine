#include "toggle.hpp"

void toggle::add_to_world(b2World * world)
{
    const settings& config = settings::get_instance();

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(this->x, this->y);

    body = world->CreateBody(&bdef);

    b2CircleShape bshape;
    bshape.m_p.Set(0.0f, 0.0f);
    bshape.m_radius = config.TOGGLE_RADIUS;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    body->CreateFixture(&fdef);
    body->SetUserData(static_cast<void*>(new collision_user_data(static_cast<void*>(this), collision_user_data_type::toggle)));
}

map* toggle::get_map(map* m)
{
    static map* m_ = m;
    return m_;
}

void toggle::step_on(ball* m)
{
    for(auto & o : tags) {
        o.step_on(*get_map(), m);
    }
}

void toggle::step_off(ball* m)
{
    for(auto & o : tags) {
        o.step_off(*get_map(), m);
    }
}

void to_json(nlohmann::json& j, const toggle& p)
{
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
        {"timer", p.timer},
        {"tags", p.tags}
    };
}

void from_json(const nlohmann::json& j, toggle& p)
{
    p.x     = j.at("x").get<float>();
    p.y     = j.at("y").get<float>();
    p.timer = j.at("timer").get<int>();
    p.tags  = j.at("tags").get<std::vector<toggle_tag>>();
}

