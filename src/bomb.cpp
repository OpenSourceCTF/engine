#include "bomb.hpp"


bomb::bomb(
    const float x,
    const float y
)
: x(x)
, y(y)
, body(nullptr)
, col_data(nullptr)
, is_alive(true)
, respawn_counter(0)
{
    const settings& config = settings::get_instance();
    ex = explosion(config.BOMB_EXPLOSION_FORCE,
                   config.BOMB_EXPLOSION_RADIUS);
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
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(this));
    body->SetUserData(static_cast<void*>(col_data.get()));

    is_alive = true;
}

//subclass b2QueryCallback
struct ExplodeAABBCallback : public b2QueryCallback
{
    std::vector<b2Body*> bodies;
  
    bool ReportFixture(b2Fixture* fixture)
	{
        bodies.push_back(fixture->GetBody()); 
        return true; // continue
    }
};

void bomb::explode()
{
    if(! is_alive) {
        return;
    }

    const settings& config = settings::get_instance();
    respawn_counter = config.BOOSTER_RESPAWN_TIME;
    std::cout << "boom: " << x << "," << y << std::endl;
    ex.explode(x,y,body->GetWorld());

    is_alive = false;
}

void to_json(nlohmann::json& j, const bomb& p)
{
    j = nlohmann::json{{"x", p.x}, {"y", p.y}};
}

void from_json(const nlohmann::json& j, bomb& p)
{
    p.x = j.at("x").get<float>();
    p.y = j.at("y").get<float>();
}
