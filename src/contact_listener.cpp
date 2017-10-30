#include "contact_listener.hpp"

const collision_user_data unknown_data;

static constexpr bool VERBOSE_CONTACT = false;

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
        if(VERBOSE_CONTACT) std::cout << "contact: ball" << std::endl;

        ball * m = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        );

        if(cdata.has_both(collision_user_data_type::ball)) {
            if(VERBOSE_CONTACT) std::cout << "contact: begin ball" << std::endl;

            ball * b = static_cast<ball*>(
                cdata.get_ptr_alt(collision_user_data_type::ball)
            );

            if(! m->powerups.empty() || ! b->powerups.empty()) {
                if(! same_color(m->type, b->type)) {
                    if(m->has_powerup(powerup_type::rollingbomb)) {
                        m->rb_explode();
                    }

                    if(b->has_powerup(powerup_type::rollingbomb)) {
                        b->rb_explode();
                    }

                    if(m->has_powerup(powerup_type::tagpro)) {
                        b->pop();
                    }
                    if(b->has_powerup(powerup_type::tagpro)) {
                        m->pop();
                    }
                }
            }
        }

        if(cdata.has(collision_user_data_type::spike)) {
            if(VERBOSE_CONTACT) std::cout << "contact: begin spike" << std::endl;

            if(m) m->pop();
        }

        if(cdata.has(collision_user_data_type::bomb)) {
            if(VERBOSE_CONTACT) std::cout << "contact: begin bomb" << std::endl;

            bomb * o = static_cast<bomb*>(
                cdata.get_ptr(collision_user_data_type::bomb)
            );

            if(o) o->explode();
        }

        if(cdata.has(collision_user_data_type::toggle)) {
            if(VERBOSE_CONTACT) std::cout << "contact: begin toggle" << std::endl;

            toggle * o = static_cast<toggle*>(
                cdata.get_ptr(collision_user_data_type::toggle)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::booster)) {
            if(VERBOSE_CONTACT) std::cout << "contact: begin booster" << std::endl;

            booster * o = static_cast<booster*>(
                cdata.get_ptr(collision_user_data_type::booster)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::powerup)) {
            if(VERBOSE_CONTACT) std::cout << "contact: begin powerup" << std::endl;

            powerup * o = static_cast<powerup*>(
                cdata.get_ptr(collision_user_data_type::powerup)
            );

            std::cout << to_string(o->type) << std::endl;

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::flag)) {
            if(VERBOSE_CONTACT) std::cout << "contact: begin flag" << std::endl;

            flag * o = static_cast<flag*>(
                cdata.get_ptr(collision_user_data_type::flag)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::portal)) {
            if(VERBOSE_CONTACT) std::cout << "contact: begin portal" << std::endl;

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
        if(VERBOSE_CONTACT) std::cout << "contact: ball" << std::endl;

        ball * m = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        );

        if(cdata.has(collision_user_data_type::toggle)) {
            if(VERBOSE_CONTACT) std::cout << "contact: left toggle" << std::endl;

            toggle * o = static_cast<toggle*>(
                cdata.get_ptr(collision_user_data_type::toggle)
            );

            if(o) o->step_off(m);
        }

        if(cdata.has(collision_user_data_type::portal)) {
            if(VERBOSE_CONTACT) std::cout << "contact: portal" << std::endl;

            portal * o = static_cast<portal*>(
                cdata.get_ptr(collision_user_data_type::portal)
            );

            if(o) o->step_off(m);
        }
    }
}
