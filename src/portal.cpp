#include "portal.hpp"


void portal::set_cooldown(const std::uint32_t x)
{
    has_cooldown = true;
    cooldown = x;
}

void portal::add_to_world(b2World * world)
{
    const settings& config = settings::get_instance();

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(this->x, this->y);

    body = world->CreateBody(&bdef);

    b2CircleShape bshape;
    bshape.m_p.Set(0.0f, 0.0f);
    bshape.m_radius = config.PORTAL_RADIUS;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    body->CreateFixture(&fdef);
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(this));
    body->SetUserData(static_cast<void*>(col_data.get()));

    is_alive = true;
}

void portal::step_on(ball* m)
{
    if(! is_alive) return;

    std::cout << "portal stepped on" << std::endl;

    if(has_destination) {
        std::cout << "move to: " << destination_id << std::endl;

        m->set_portal_transport(destination_ptr);
        is_alive = false;

        if(has_cooldown) {
            is_cooling_down = true;
            cooldown_counter = cooldown;
        }
    }
}

void portal::step_off(ball* m)
{
    std::cout << "portal stepped off" << std::endl;
}

void to_json(nlohmann::json& j, const portal& p)
{
    j = nlohmann::json{
        {"x", p.x}, {"y", p.y},
        {"has_cooldown", p.has_cooldown}, {"cooldown", p.cooldown},
        {"has_destination", p.has_destination}, {"destination_id", p.destination_id}
    };
}

void from_json(const nlohmann::json& j, portal& p)
{
    p.x = j.at("x").get<float>();
    p.y = j.at("y").get<float>();

    p.has_cooldown = j.at("has_cooldown").get<bool>();
    if(p.has_cooldown) {
        p.cooldown = j.at("cooldown").get<int>();
    }

    p.has_destination = j.at("has_destination").get<bool>();
    if(p.has_destination) {
        p.destination_id  = j.at("destination_id").get<int>();
    }
}

