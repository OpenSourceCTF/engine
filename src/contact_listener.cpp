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
        std::cout << "contact: ball" << std::endl;

        ball * m = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        );

        if(cdata.has_both(collision_user_data_type::ball)) {
            std::cout << "contact: begin ball" << std::endl;

            ball * b = static_cast<ball*>(
                cdata.get_ptr_alt(collision_user_data_type::ball)
            );

            if(! m->powerups.empty() || ! b->powerups.empty()) {
                std::cout << "POWERTIME" << std::endl;
                if(m->has_powerup(powerup_type::rollingbomb)) {
                    std::cout << "rollingboom" << std::endl;
                }

                if(b->has_powerup(powerup_type::rollingbomb)) {
                    std::cout << "rollingboom" << std::endl;
                }

                if(m->has_powerup(powerup_type::tagpro)) {
                    b->pop();
                }
                if(b->has_powerup(powerup_type::tagpro)) {
                    m->pop();
                }
            }
        }

        if(cdata.has(collision_user_data_type::spike)) {
            std::cout << "contact: begin spike" << std::endl;

            if(m) m->pop();
        }

        if(cdata.has(collision_user_data_type::bomb)) {
            std::cout << "contact: begin bomb" << std::endl;

            bomb * o = static_cast<bomb*>(
                cdata.get_ptr(collision_user_data_type::bomb)
            );

            if(o) o->explode(m);
        }

        if(cdata.has(collision_user_data_type::toggle)) {
            std::cout << "contact: begin toggle" << std::endl;

            toggle * o = static_cast<toggle*>(
                cdata.get_ptr(collision_user_data_type::toggle)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::booster)) {
            std::cout << "contact: begin booster" << std::endl;

            booster * o = static_cast<booster*>(
                cdata.get_ptr(collision_user_data_type::booster)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::powerup)) {
            std::cout << "contact: begin powerup" << std::endl;

            powerup * o = static_cast<powerup*>(
                cdata.get_ptr(collision_user_data_type::powerup)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::flag)) {
            std::cout << "contact: begin flag" << std::endl;

            flag * o = static_cast<flag*>(
                cdata.get_ptr(collision_user_data_type::flag)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::portal)) {
            std::cout << "contact: begin portal" << std::endl;

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
        std::cout << "contact: ball" << std::endl;

        ball * m = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        );

        if(cdata.has(collision_user_data_type::toggle)) {
            std::cout << "contact: left toggle" << std::endl;

            toggle * o = static_cast<toggle*>(
                cdata.get_ptr(collision_user_data_type::toggle)
            );

            if(o) o->step_off(m);
        }

        if(cdata.has(collision_user_data_type::portal)) {
            std::cout << "contact: portal" << std::endl;

            portal * o = static_cast<portal*>(
                cdata.get_ptr(collision_user_data_type::portal)
            );

            if(o) o->step_off(m);
        }
    }
}
