#ifndef ML_CONTACT_LISTENER_HPP
#define ML_CONTACT_LISTENER_HPP

#include <Box2D/Box2D.h>
#include "collision_user_data.hpp"
#include "ball.hpp"
#include "wall.hpp"
#include "spike.hpp"


// this holds two collision user datas together
// representing a contact between two things
struct cslot
{
    std::array<collision_user_data, 2> held;

    cslot(const std::array<collision_user_data, 2> held)
    : held(held)
    {}

    bool has(const collision_user_data_type type) const
    {
        return held[0].type == type || held[1].type == type;
    }

    bool has_both(const collision_user_data_type type) const
    {
        return held[0].type == type && held[1].type == type;
    }

    // only call this if you've verified with has
    // otherwise (very) unsafe
    void* get_ptr(const collision_user_data_type type) const
    {
        return held[0].type == type ? held[0].ptr : held[1].ptr;
    }
};

const collision_user_data unknown_data(nullptr, collision_user_data_type::unknown);

class contact_listener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
	{
        const collision_user_data* user_data_a = reinterpret_cast<collision_user_data*>(
            contact->GetFixtureA()->GetBody()->GetUserData()
        );
        const collision_user_data* user_data_b = reinterpret_cast<collision_user_data*>(
            contact->GetFixtureB()->GetBody()->GetUserData()
        );


        const cslot cdata({
            (user_data_a) ? *user_data_a : unknown_data,
            (user_data_b) ? *user_data_b : unknown_data
        });

        if(cdata.has(collision_user_data_type::ball)) {
            std::cout << "has ball" << std::endl;

            ball * m = reinterpret_cast<ball*>(
                cdata.get_ptr(collision_user_data_type::ball)
            );

            if(cdata.has_both(collision_user_data_type::ball)) {
                std::cout << "touching another ball" << std::endl;

                // todo
                // this is where we'd have powerups affect
                // or deal with flags
            }

            if(cdata.has(collision_user_data_type::spike)) {
                std::cout << "has spike" << std::endl;

                if(m) {
                    m->pop();
                }
            }

            if(cdata.has(collision_user_data_type::bomb)) {
                std::cout << "has bomb" << std::endl;

                bomb * o = reinterpret_cast<bomb*>(
                    cdata.get_ptr(collision_user_data_type::bomb)
                );

                if(o) {
                    o->explode();
                }
            }
        }
    }

    void EndContact(b2Contact* contact)
    {
        const collision_user_data * user_data_a = reinterpret_cast<collision_user_data*>(
            contact->GetFixtureA()->GetBody()->GetUserData()
        );
        const collision_user_data * user_data_b = reinterpret_cast<collision_user_data*>(
            contact->GetFixtureB()->GetBody()->GetUserData()
        );

        const cslot cdata({
            (user_data_a) ? *user_data_a : unknown_data,
            (user_data_b) ? *user_data_b : unknown_data
        });
    }
};


#endif

