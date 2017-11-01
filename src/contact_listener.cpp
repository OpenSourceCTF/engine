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
        if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: ball");

        ball * m = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        );

        if(cdata.has_both(collision_user_data_type::ball)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin ball");

            ball * b = static_cast<ball*>(
                cdata.get_ptr_alt(collision_user_data_type::ball)
            );

            if(! same_color(m->type, b->type)) {
                bool disable_flag_check = false;

                if(! m->powerups.empty() || ! b->powerups.empty()) {
                    if(m->has_powerup(powerup_type::rollingbomb)) {
                        disable_flag_check = true;
                        m->rb_explode();
                    }

                    if(b->has_powerup(powerup_type::rollingbomb)) {
                        disable_flag_check = true;
                        b->rb_explode();
                    }

                    if(m->has_powerup(powerup_type::tagpro)) {
                        b->pop();
                    }
                    if(b->has_powerup(powerup_type::tagpro)) {
                        m->pop();
                    }
                }

                if(! disable_flag_check && (! m->flags.empty() || ! b->flags.empty())) {
                    if(m->has_flag(inv_corresponding_color<flag_type>(m->type))) {
                        m->pop();
                    }

                    if(b->has_flag(inv_corresponding_color<flag_type>(b->type))) {
                        b->pop();
                    }
                }
            }
        }

        if(cdata.has(collision_user_data_type::spike)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin spike");

            if(m) m->pop();
        }

        if(cdata.has(collision_user_data_type::gate)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin gate");

            gate* o = static_cast<gate*>(
                    cdata.get_ptr(collision_user_data_type::gate)
            );

            ball * b = static_cast<ball*>(
                cdata.get_ptr(collision_user_data_type::ball)
            );

            if(o && b) {
                b->in_gate_ptrs.insert(o);
            }
        }

        if(cdata.has(collision_user_data_type::bomb)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin bomb");

            bomb * o = static_cast<bomb*>(
                cdata.get_ptr(collision_user_data_type::bomb)
            );

            if(o) o->explode();
        }

        if(cdata.has(collision_user_data_type::toggle)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin toggle");

            toggle * o = static_cast<toggle*>(
                cdata.get_ptr(collision_user_data_type::toggle)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::booster)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin booster");

            booster * o = static_cast<booster*>(
                cdata.get_ptr(collision_user_data_type::booster)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::powerup)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin powerup");

            powerup * o = static_cast<powerup*>(
                cdata.get_ptr(collision_user_data_type::powerup)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::flag)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin flag");

            flag * o = static_cast<flag*>(
                cdata.get_ptr(collision_user_data_type::flag)
            );

            if(o) o->step_on(m);
        }

        if(cdata.has(collision_user_data_type::portal)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin portal");

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
        if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: ball");

        ball * m = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        );

        if(cdata.has(collision_user_data_type::toggle)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: left toggle");

            toggle * o = static_cast<toggle*>(
                cdata.get_ptr(collision_user_data_type::toggle)
            );

            if(o) o->step_off(m);
        }

        if(cdata.has(collision_user_data_type::portal)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: portal");

            portal * o = static_cast<portal*>(
                cdata.get_ptr(collision_user_data_type::portal)
            );

            if(o) o->step_off(m);
        }

        if(cdata.has(collision_user_data_type::gate)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: left gate");

            gate* o = static_cast<gate*>(
                    cdata.get_ptr(collision_user_data_type::gate)
            );

            ball * b = static_cast<ball*>(
                cdata.get_ptr(collision_user_data_type::ball)
            );

            if(o && b) {
                b->in_gate_ptrs.erase(o);
            }
        }
    }
}
