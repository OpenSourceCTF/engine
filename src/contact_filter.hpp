#ifndef ML_CONTACT_FILTER_HPP
#define ML_CONTACT_FILTER_HPP

#include <Box2D/Box2D.h>
#include <spdlog/spdlog.h>
#include "cslot.hpp"
#include "collision_user_data.hpp"
#include "ball.hpp"
#include "chain.hpp"
#include "spike.hpp"
#include "toggle.hpp"
#include "booster.hpp"
#include "powerup.hpp"
#include "flag.hpp"
#include "portal.hpp"

struct ball;
struct chain;
struct gate;
struct bomb;
struct spike;
struct toggle;
struct booster;
struct powerup;
struct flag;
struct portal;


class contact_filter : public b2ContactFilter
{
    bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
};


#endif

