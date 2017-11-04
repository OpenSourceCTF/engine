#include "booster.hpp"

thread_local std::size_t booster::id_counter = 0;

booster::booster() {}

booster::booster(
    const float x,
    const float y,
    const booster_type type
)
: id(id_counter++)
, x(x)
, y(y)
, type(type)
, body(nullptr)
, col_data(nullptr)
, is_alive(true)
, respawn_counter(0)
{}

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
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(collision_user_data_type::booster, this));
    body->SetUserData(static_cast<void*>(col_data.get()));

    is_alive = true;
}

booster::~booster()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
    }
}

void booster::step_on(ball* m)
{
    if(! is_alive) {
        return;
    }
    spdlog::get("game")->debug("booster stepped on");

    const settings& config = settings::get_instance();
    respawn_counter = config.BOOSTER_RESPAWN_TIME;

    switch(type) {
        case booster_type::all: boost_ball(m); break;
        case booster_type::red:  
        case booster_type::blue:
           if(same_color(type, m->type)) boost_ball(m);
           else return; // prevents is_alive from being switched
           break;
    }

    is_alive = false;
}

void booster::boost_ball(ball* m)
{
    m->player_ptr->g->add_server_event(server_event(server_event_ball_boosted(m, this)));
    m->get_boosted();
}

void to_json(nlohmann::json& j, const booster& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}, {"type", to_string(p.type)}};
}

void from_json(const nlohmann::json& j, booster& p)
{
    p = booster(
        j.at("x").get<float>(),
        j.at("y").get<float>(),
        booster_type_from_string(j.at("type").get<std::string>())
    );
}
