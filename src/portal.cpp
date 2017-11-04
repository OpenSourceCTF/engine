#include "portal.hpp"

thread_local std::size_t portal::id_counter = 0;


portal::portal() {}
portal::portal(
    const float x,
    const float y
)
: id(id_counter++)
, x(x)
, y(y)
, has_cooldown(false)
, is_cooling_down(false)
, cooldown_counter(0)
, has_destination(false)
, destination_id(0)
, destination_ptr(nullptr)
, body(nullptr)
, col_data(nullptr)
, is_alive(true)
{}

portal::~portal()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
    }
}


void portal::set_cooldown(const std::uint32_t x)
{
    has_cooldown = true;
    cooldown = x;
}

void portal::set_destination(const std::size_t id)
{
    has_destination = true;
    destination_id = id;
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
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(collision_user_data_type::portal, this));
    body->SetUserData(static_cast<void*>(col_data.get()));

    is_alive = true;
}

void portal::step_on(ball* m)
{
    spdlog::get("game")->debug("portal stepped on");

    portal* p = this;

    while(p->has_destination) {
        if(! p->is_alive) return;

        spdlog::get("game")->debug("move to: {0:d}", p->destination_id);
        portal* dest = p->destination_ptr; 

        m->set_portal_transport(dest);

        const auto disable_portal = [](portal* o) {
            o->is_alive = false;

            if(o->has_cooldown) {
                o->is_cooling_down = true;
                o->cooldown_counter = o->cooldown;
            }
        };

        disable_portal(p);
        disable_portal(dest);

        p = dest;
    }
}

void portal::step_off(ball* m)
{
    spdlog::get("game")->debug("portal stepped off");
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
    const settings& config = settings::get_instance();

    p = portal(
        j.at("x").get<float>(),
        j.at("y").get<float>()
    );

    if(j.at("has_cooldown").get<bool>()) {
        const int c = j.at("cooldown").get<int>();
        p.set_cooldown(c ? c : config.PORTAL_RESPAWN_TIME);
    }

    if(j.at("has_destination").get<bool>()) {
        p.set_destination(j.at("destination_id").get<int>());
    }
}

