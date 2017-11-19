#include "gravwell.hpp"
#include <json/json.hpp>

gravwell::gravwell(){}
gravwell::gravwell(
    const float x,
    const float y,
    const float r,
    const float f
)
: x(x)
, y(y)
, r(r)
, f(f)
, body(nullptr)
, col_data(nullptr)
{}

gravwell::~gravwell()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
    }
}

void gravwell::add_to_world(b2World * world)
{
    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(this->x, this->y);

    body = world->CreateBody(&bdef);

    b2CircleShape bshape;
    bshape.m_p.Set(0.0f, 0.0f);
    bshape.m_radius = r;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    body->CreateFixture(&fdef);
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(collision_user_data_type::gravwell, this));
    body->SetUserData(static_cast<void*>(col_data.get()));
}

void gravwell::suck(b2World* world)
{
    const b2Vec2 pos(body->GetPosition());
    GravWellAABBCallback callback;
    b2AABB aabb;
    aabb.lowerBound = b2Vec2(pos.x - r, pos.y - r);
    aabb.upperBound = b2Vec2(pos.x + r, pos.y + r);

    world->QueryAABB(&callback, aabb);

    for(auto && m : callback.bodies) {
        const b2Vec2 mpos = m->GetPosition();
        const float d = dist(pos.x, pos.y, mpos.x, mpos.y);

        if(d >= r) {
            continue;
        }

        const float force = map_val(
            d,
            0.0f, r,
            f, 0.0f
        );

        b2Vec2 force_vec(pos.x - mpos.x, pos.y - mpos.y);
        force_vec.Normalize();
        force_vec *= force;

        m->ApplyForce(
            force_vec,
            m->GetWorldCenter(),
            true
        );
    }
}

void to_json(nlohmann::json& j, const gravwell& p)
{
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
        {"r", p.r},
        {"f", p.f},
    };
}

void from_json(const nlohmann::json& j, gravwell& p)
{
    p = gravwell(
        j.at("x").get<float>(),
        j.at("y").get<float>(),
        j.at("r").get<float>(),
        j.at("f").get<float>()
    );
}
