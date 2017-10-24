#include "powerup.hpp"

powerup::powerup()
: possible_types({
    powerup_type::tagpro,
    powerup_type::jukejuice,
    powerup_type::rollingbomb
})
{}

powerup::powerup(
    const float x,
    const float y
)
: x(x)
, y(y)
, body(nullptr)
, col_data(nullptr)
, is_alive(true)
, respawn_counter(0)
, possible_types({
    powerup_type::tagpro,
    powerup_type::jukejuice,
    powerup_type::rollingbomb
})
, type(get_random_type())
{}

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
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(this));
    body->SetUserData(static_cast<void*>(col_data.get()));

    is_alive = true;
}

void powerup::step_on(ball* m)
{
    if(! is_alive) {
        return;
    }
    std::cout << "powerup stepped on: " << to_string(type) << std::endl;
    const settings& config = settings::get_instance();

    m->add_powerup(type);

    respawn_counter = config.POWERUP_RESPAWN_TIME;
    is_alive = false;
}

powerup_type powerup::get_random_type()
{
    if(possible_types.empty()) {
        std::cerr << "error: powerup get_random_type called but no types available" << std::endl;
        return powerup_type::tagpro;
    }

    return possible_types.at(
        std::uniform_int_distribution<int>(
            0, possible_types.size() - 1
        )(random_util::get_instance().eng)
    );
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
