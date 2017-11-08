#include "powerup.hpp"
#include <json/json.hpp>

thread_local std::size_t powerup::id_counter = 0;

powerup::powerup()
: possible_types({
    powerup_type::tagpro,
    powerup_type::jukejuice,
    powerup_type::rollingbomb
})
{}

powerup::powerup(
    const float x,
    const float y,
    const std::vector<powerup_type> possible_types
)
: id(id_counter++)
, x(x)
, y(y)
, body(nullptr)
, col_data(nullptr)
, is_alive(true)
, respawn_counter(0)
, possible_types(possible_types)
, type(get_random_type())
{}

powerup::~powerup()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
    }
}

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
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(collision_user_data_type::powerup, this));
    body->SetUserData(static_cast<void*>(col_data.get()));

    is_alive = true;
}

void powerup::step_on(ball* m)
{
    if(! is_alive) {
        return;
    }
    spdlog::get("game")->debug("powerup stepped on ", to_string(type));
    const settings& config = settings::get_instance();

    m->add_powerup(type);

    respawn_counter = config.POWERUP_RESPAWN_TIME;
    is_alive = false;
    game.add_server_event(server_event(server_event_ball_powerup(m, this)));
}

powerup_type powerup::get_random_type()
{
    if(possible_types.empty()) {
        spdlog::get("game")->info("powerup get_random_type called but no types available");
        return powerup_type::tagpro;
    }

    return possible_types[
        std::uniform_int_distribution<std::size_t>(
            0, possible_types.size() - 1
        )(random_util::get_instance().eng)
    ];
}

void to_json(nlohmann::json& j, const powerup& p)
{
    std::vector<std::string> types;
    for(auto && o : p.possible_types) {
        types.emplace_back(to_string(o));
    };

    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
        {"types", types}
    };
}

void from_json(const nlohmann::json& j, powerup& p)
{
    std::vector<powerup_type> possible_types;
    const std::vector<std::string> types = j.at("types").get<std::vector<std::string>>();
    for(auto && o : types) {
        possible_types.emplace_back(powerup_type_from_string(o));
    }

    p = powerup(
        j.at("x").get<float>(),
        j.at("y").get<float>(),
        possible_types
    );

}
