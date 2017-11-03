#include "flag.hpp"

flag::~flag()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
    }
}
void flag::add_to_world(b2World * world)
{
    const settings& config = settings::get_instance();

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(this->x, this->y);

    body = world->CreateBody(&bdef);

    b2CircleShape bshape;
    bshape.m_p.Set(0.0f, 0.0f);
    bshape.m_radius = config.FLAG_RADIUS;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    body->CreateFixture(&fdef);
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(collision_user_data_type::flag, this));
    body->SetUserData(static_cast<void*>(col_data.get()));

    is_alive = true;
}

void flag::step_on(ball* m)
{
    if(! is_alive) return;

    spdlog::get("game")->debug("flag stepped on");
    if(! same_color(type, m->type)) {
        m->add_flag(this);
        is_alive = false;
    } else if(! m->flags.empty()) {
        m->score();
    }
}

void to_json(nlohmann::json& j, const flag& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}, {"type", to_string(p.type)}};
}

void from_json(const nlohmann::json& j, flag& p)
{
    p = flag(
        j.at("x").get<float>(),
        j.at("y").get<float>(),
        flag_type_from_string(j.at("type").get<std::string>())
    );
}
