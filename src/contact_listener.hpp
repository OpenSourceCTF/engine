#ifndef ML_CONTACT_LISTENER_HPP
#define ML_CONTACT_LISTENER_HPP

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


class contact_listener : public b2ContactListener
{
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);

    void begin_ball(ball* a, ball* b, b2WorldManifold manifold);
    void begin_ball(ball* a, chain* b, b2WorldManifold manifold);
    void begin_ball(ball* a, spike* b);
    void begin_ball(ball* a, gate* b);
    void begin_ball(ball* a, bomb* b, b2WorldManifold manifold);
    void begin_ball(ball* a, toggle* b);
    void begin_ball(ball* a, booster* b);
    void begin_ball(ball* a, powerup* b);
    void begin_ball(ball* a, flag* b);
    void begin_ball(ball* a, portal* b);

    void end_ball(ball* a, ball* b);
    // void end_ball(ball* a, chain* b, b2WorldManifold manifold);
    void end_ball(ball* a, toggle* b);
    void end_ball(ball* a, portal* b);
    void end_ball(ball* a, gate* b);
};


#endif

