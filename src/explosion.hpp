#ifndef ML_EXPLOSION_HPP
#define ML_EXPLOSION_HPP

#include <Box2D/Box2D.h>
#include "util.hpp"

struct explosion
{
    const float radius;
    const float force;

    explosion() = delete;
    explosion(
        const float radius,
        const float force
    )
    : radius(radius)
    , force(force)
    {}

    void explode(
        const b2Vec2 pos,
        b2World* world
    );
};

#endif

