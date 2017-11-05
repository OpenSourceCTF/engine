#include "toggle.hpp"
#include <json/json.hpp>

thread_local std::size_t toggle::id_counter = 0;

toggle::toggle(){}

toggle::toggle(
    const float x,
    const float y,
    const std::uint32_t timer,
    const std::vector<toggle_tag> tags
)
: id(id_counter++)
, x(x)
, y(y)
, timer(timer)
, tags(tags)
, body(nullptr)
, col_data(nullptr)
{}

toggle::~toggle()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
    }
}

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
    col_data = std::shared_ptr<collision_user_data>(
        new collision_user_data(collision_user_data_type::toggle, this)
    );
    body->SetUserData(static_cast<void*>(col_data.get()));
}

void toggle::step_on(ball* b)
{
    for(auto & o : tags) {
        o.step_on(b);
    }

    game.add_server_event(server_event(server_event_toggle_on(b, this)));
}

void toggle::step_off(ball* b)
{
    for(auto & o : tags) {
        o.step_off(b);
    }

    game.add_server_event(server_event(server_event_toggle_off(b, this)));
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
    p = toggle(
        j.at("x").get<float>(),
        j.at("y").get<float>(),
        j.at("timer").get<int>(),
        j.at("tags").get<std::vector<toggle_tag>>()
    );
}

