#include "bomb.hpp"
#include <json/json.hpp>

thread_local std::size_t bomb::id_counter = 0;

bomb::bomb() {}
bomb::bomb(
    const float x,
    const float y
)
: id(id_counter++)
, x(x)
, y(y)
, body(nullptr)
, col_data(nullptr)
, is_alive(true)
, respawn_counter(0)
{
}

bomb::~bomb()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
    }
}

void bomb::add_to_world(b2World * world)
{
    const settings& config = settings::get_instance();

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(this->x, this->y);

    body = world->CreateBody(&bdef);

    b2CircleShape bshape;
    bshape.m_p.Set(0.0f, 0.0f);
    bshape.m_radius = config.BOMB_RADIUS;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    body->CreateFixture(&fdef);
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(collision_user_data_type::bomb, this));
    body->SetUserData(static_cast<void*>(col_data.get()));

    is_alive = true;
}

void bomb::explode()
{
    if(! is_alive) {
        return;
    }

    const settings& config = settings::get_instance();
    respawn_counter = config.BOOSTER_RESPAWN_TIME;
    explosion(config.BOMB_EXPLOSION_RADIUS, config.BOMB_EXPLOSION_FORCE)
        .explode(body->GetPosition(), body->GetWorld());

    is_alive = false;
    game.add_server_event(server_event(server_event_bomb_explosion(this)));
}

void to_json(nlohmann::json& j, const bomb& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}};
}

void from_json(const nlohmann::json& j, bomb& p)
{
    p = bomb(
        j.at("x").get<float>(),
        j.at("y").get<float>()
    );
}
