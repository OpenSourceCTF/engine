#include "explosion.hpp"

explosion::explosion(
    const float radius,
    const float force
)
: radius(radius)
, force(force)
{}

void explosion::explode(
    const b2Vec2 pos,
    b2World* world
) {
    ExplodeAABBCallback callback;
    b2AABB aabb;
    aabb.lowerBound = b2Vec2(pos.x - radius, pos.y - radius);
    aabb.upperBound = b2Vec2(pos.x + radius, pos.y + radius);

    world->QueryAABB(&callback, aabb);

    for(auto & m : callback.bodies) {
        const b2Vec2 mpos = m->GetPosition();

        const float d = dist(pos.x, pos.y, mpos.x, mpos.y);

        if(d >= radius) {
            continue;
        }

        const float f = map_val(
            d,
            0.0f, radius,
            force, 0.0f
        );

        b2Vec2 force_vec(mpos.x - pos.x, mpos.y - pos.y);
        force_vec.Normalize();
        force_vec *= f;

        m->ApplyForce(
            force_vec,
            m->GetWorldCenter(),
            true
        );
    }
}
