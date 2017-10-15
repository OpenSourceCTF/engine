#ifndef ML_CONTACT_LISTENER_HPP
#define ML_CONTACT_LISTENER_HPP

#include <Box2D/Box2D.h>
#include "cslot.hpp"
#include "collision_user_data.hpp"
#include "ball.hpp"
#include "wall.hpp"
#include "spike.hpp"
#include "toggle.hpp"
#include "booster.hpp"
#include "flag.hpp"


class contact_listener : public b2ContactListener
{
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
};


#endif

