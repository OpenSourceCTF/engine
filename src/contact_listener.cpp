#include "contact_listener.hpp"

struct toggle;

const collision_user_data unknown_data;

void contact_listener::BeginContact(b2Contact* contact)
{
    const collision_user_data* user_data_a = static_cast<collision_user_data*>(
        contact->GetFixtureA()->GetBody()->GetUserData()
    );
    const collision_user_data* user_data_b = static_cast<collision_user_data*>(
        contact->GetFixtureB()->GetBody()->GetUserData()
    );


    const cslot cdata({
        (user_data_a) ? *user_data_a : unknown_data,
        (user_data_b) ? *user_data_b : unknown_data
    });

    if(cdata.has(collision_user_data_type::ball)) {
        std::cout << "has ball" << std::endl;

        ball * m = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        );

        if(cdata.has_both(collision_user_data_type::ball)) {
            std::cout << "touching another ball" << std::endl;

            // todo
            // this is where we'd have powerups affect
            // or deal with popping flag carrier
        }

        if(cdata.has(collision_user_data_type::spike)) {
            std::cout << "has spike" << std::endl;

            if(m) m->pop();
        }

        if(cdata.has(collision_user_data_type::bomb)) {
            std::cout << "has bomb" << std::endl;

            bomb * o = static_cast<bomb*>(
                cdata.get_ptr(collision_user_data_type::bomb)
            );

            if(o) o->explode();
        }

        if(cdata.has(collision_user_data_type::toggle)) {
            std::cout << "has toggle" << std::endl;

            toggle * o = static_cast<toggle*>(
                cdata.get_ptr(collision_user_data_type::toggle)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::booster)) {
            std::cout << "has booster" << std::endl;

            booster * o = static_cast<booster*>(
                cdata.get_ptr(collision_user_data_type::booster)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::powerup)) {
            std::cout << "has powerup" << std::endl;

            powerup * o = static_cast<powerup*>(
                cdata.get_ptr(collision_user_data_type::powerup)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::flag)) {
            std::cout << "has flag" << std::endl;

            flag * o = static_cast<flag*>(
                cdata.get_ptr(collision_user_data_type::flag)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::portal)) {
            std::cout << "has portal" << std::endl;

            portal * o = static_cast<portal*>(
                cdata.get_ptr(collision_user_data_type::portal)
            );

            if(o) o->step_on(m);
        }
    }
}

void contact_listener::EndContact(b2Contact* contact)
{
    const collision_user_data * user_data_a = static_cast<collision_user_data*>(
        contact->GetFixtureA()->GetBody()->GetUserData()
    );
    const collision_user_data * user_data_b = static_cast<collision_user_data*>(
        contact->GetFixtureB()->GetBody()->GetUserData()
    );

    const cslot cdata({
        (user_data_a) ? *user_data_a : unknown_data,
        (user_data_b) ? *user_data_b : unknown_data
    });

    if(cdata.has(collision_user_data_type::ball)) {
        std::cout << "has ball" << std::endl;

        ball * m = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        );

        if(cdata.has(collision_user_data_type::toggle)) {
            std::cout << "has toggle" << std::endl;

            toggle * o = static_cast<toggle*>(
                cdata.get_ptr(collision_user_data_type::toggle)
            );

            if(o) o->step_off(m);
        }
    }
}
