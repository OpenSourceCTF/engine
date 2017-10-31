#include "explosion.hpp"

struct ExplodeAABBCallback : public b2QueryCallback
{
    std::vector<b2Body*> bodies;
  
    bool ReportFixture(b2Fixture* fixture)
	{
        if(fixture->GetBody()->GetMass() > 0)
        bodies.push_back(fixture->GetBody()); 
        return true; // continue
    }
};

void explosion::explode(const b2Vec2 pos, b2World* world) {
    ExplodeAABBCallback callback;
    b2AABB aabb;
    aabb.lowerBound = b2Vec2(pos.x - radius, pos.y - radius);
    aabb.upperBound = b2Vec2(pos.x + radius, pos.y + radius);

    world->QueryAABB(&callback, aabb);

    for(auto & m : callback.bodies) {
        const float d = dist(pos.x, pos.y, m->GetPosition().x, m->GetPosition().y);

        if(d >= radius) {
            continue;
        }

        const float f = map_val(
            d,
            0, radius,
            force, 0
        );

        const b2Vec2 bpos = m->GetPosition();

        b2Vec2 force_vec(bpos.x - pos.x, bpos.y - pos.y);
        force_vec.Normalize();
        force_vec *= f;
        m->ApplyForce(
            force_vec,
            m->GetWorldCenter(),
            true
        );
    }
}
