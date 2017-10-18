#include "bomb.hpp"

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
    body->SetUserData(static_cast<void*>(col_data));
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
    const settings& config = settings::get_instance();
    std::cout << "boom: " << x << "," << y << std::endl;

    ExplodeAABBCallback callback;
    b2AABB aabb;
    aabb.lowerBound = b2Vec2(x - config.BOMB_EXPLOSION_RADIUS, y - config.BOMB_EXPLOSION_RADIUS);
    aabb.upperBound = b2Vec2(x + config.BOMB_EXPLOSION_RADIUS, y + config.BOMB_EXPLOSION_RADIUS);

    body->GetWorld()->QueryAABB(&callback, aabb);

    for(auto & m : callback.bodies) {
        const float d = dist(x, y, m->GetPosition().x, m->GetPosition().y);

        if(d >= config.BOMB_EXPLOSION_RADIUS) {
            continue;
        }

        const float f = map_val(
            d,
            0, config.BOMB_EXPLOSION_RADIUS,
            config.BOMB_EXPLOSION_FORCE, 0
        );

        const b2Vec2 apos = body->GetPosition();
        const b2Vec2 bpos = m->GetPosition();

        const float a = std::atan2(bpos.y - apos.y, bpos.x - apos.x);

        m->ApplyForce(
            b2Vec2(f * std::cos(a), f * std::sin(a)),
            m->GetWorldCenter(),
            true
        );
    }
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
