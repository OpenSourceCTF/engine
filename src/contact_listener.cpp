#include "contact_listener.hpp"

const collision_user_data unknown_data;

static constexpr bool VERBOSE_CONTACT = true;

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

        // TODO
        // extract this out to a nice function we can unit test
        // since all the interactions a bit tricky
        if(cdata.has_both(collision_user_data_type::ball)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin ball");

            ball * b = static_cast<ball*>(
                cdata.get_ptr_alt(collision_user_data_type::ball)
            );

            if(! same_color(m->type, b->type)) {
                bool disable_flag_check_a = m->grab_invincibility_counter > 0;
                bool disable_flag_check_b = b->grab_invincibility_counter > 0;

                if(! m->powerups.empty() || ! b->powerups.empty()) {
                    if(m->has_powerup(powerup_type::rollingbomb)) {
                        disable_flag_check_a = true;
                        m->rb_explode();
                    }

                    if(b->has_powerup(powerup_type::rollingbomb)) {
                        disable_flag_check_b = true;
                        b->rb_explode();
                    }

                    if(m->has_powerup(powerup_type::tagpro)) {
                        b->pop();
                        m->player_ptr->stats.tags++;
                        b->player_ptr->stats.popped_tagged++;
                    }
                    if(b->has_powerup(powerup_type::tagpro)) {
                        m->pop();
                        b->player_ptr->stats.tags++;
                        m->player_ptr->stats.popped_tagged++;
                    }
                }

                if(! disable_flag_check_a && ! m->flags.empty()) {
                    if(m->has_flag(inv_corresponding_color<flag_type>(m->type))) {
                        m->pop();
                        b->player_ptr->stats.flag_returns++;
                    } else if(m->has_flag(flag_type::neutral)) {
                        b->take_flags(m, flag_type::neutral);
                        m->pop();
                    }
                }

                if(! disable_flag_check_b && ! b->flags.empty()) {
                    if(b->has_flag(inv_corresponding_color<flag_type>(b->type))) {
                        b->pop();
                        m->player_ptr->stats.flag_returns++;
                    } else if(b->has_flag(flag_type::neutral)) {
                        m->take_flags(b, flag_type::neutral);
                        b->pop();
                    }
                }
            }
        }

        if(cdata.has(collision_user_data_type::spike)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin spike");

            if(m) {
                m->pop();
                m->player_ptr->stats.popped_spikes++;
            }
        }

        if(cdata.has(collision_user_data_type::gate)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin gate");

            gate* o = static_cast<gate*>(
                cdata.get_ptr(collision_user_data_type::gate)
            );

            if(o) {
                m->in_gate_ptrs.insert(o);
            }
        }

        if(cdata.has(collision_user_data_type::bomb)) {
            if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin bomb");

            bomb * o = static_cast<bomb*>(
                cdata.get_ptr(collision_user_data_type::bomb)
            );

            if(o) {
                o->explode();
                m->player_ptr->stats.bombs_hit++;
            }
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

        // check tiles
        if(m->type == ball_type::red) {
            if(cdata.has(collision_user_data_type::speed_red)) {
                m->on_tile_speed_counter++;
            }
            if(cdata.has(collision_user_data_type::endzone_red)) {
                m->on_tile_endzone_counter++;
            }
        }

        if(m->type == ball_type::blue) {
            if(cdata.has(collision_user_data_type::speed_blue)) {
                m->on_tile_speed_counter++;
            }
            if(cdata.has(collision_user_data_type::endzone_blue)) {
                m->on_tile_endzone_counter++;
            }
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

            gate * o = static_cast<gate*>(
                cdata.get_ptr(collision_user_data_type::gate)
            );

            if(o) {
                m->in_gate_ptrs.erase(o);
            }
        }

        if(m->type == ball_type::red) {
            if(cdata.has(collision_user_data_type::speed_red)) {
                m->on_tile_speed_counter--;
            }
            if(cdata.has(collision_user_data_type::endzone_red)) {
                m->on_tile_endzone_counter--;
            }
        }

        if(m->type == ball_type::blue) {
            if(cdata.has(collision_user_data_type::speed_blue)) {
                m->on_tile_speed_counter--;
            }
            if(cdata.has(collision_user_data_type::endzone_blue)) {
                m->on_tile_endzone_counter--;
            }
        }
    }
}
