#ifndef ML_EXPLOSION_HPP
#define ML_EXPLOSION_HPP

#include <Box2D/Box2D.h>
#include "util.hpp"

struct explosion
{
    float radius;
    float force;

    explosion() { }
    explosion(const float radius,
              const float force) :
        radius(radius),
        force(force)
    { }

    void explode(float x, float y, b2World* world);
};

#endif

