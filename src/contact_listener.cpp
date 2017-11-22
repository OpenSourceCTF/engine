#include "contact_listener.hpp"

const collision_user_data unknown_data;

static constexpr bool VERBOSE_CONTACT = true;

void contact_listener::BeginContact(b2Contact* contact)
{
    b2WorldManifold manifold;
    contact->GetWorldManifold(&manifold);

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

        ball * a = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        );

        if(cdata.has_both(collision_user_data_type::ball)) {
            begin_ball(a,
                static_cast<ball*>(cdata.get_ptr_alt(collision_user_data_type::ball)),
                manifold
            );
        }

        if(cdata.has(collision_user_data_type::chain)) {
            begin_ball(a,
                static_cast<chain*>(cdata.get_ptr(collision_user_data_type::chain)),
                manifold
            );
        }

        if(cdata.has(collision_user_data_type::spike)) {
            begin_ball(a, static_cast<spike*>(cdata.get_ptr(collision_user_data_type::spike)));
        }

        if(cdata.has(collision_user_data_type::gate)) {
            begin_ball(a, static_cast<gate*>(cdata.get_ptr(collision_user_data_type::gate)));
        }

        if(cdata.has(collision_user_data_type::bomb)) {
            begin_ball(
                a, static_cast<bomb*>(cdata.get_ptr(collision_user_data_type::bomb)),
                manifold
            );
        }

        if(cdata.has(collision_user_data_type::toggle)) {
            begin_ball(a, static_cast<toggle*>(cdata.get_ptr(collision_user_data_type::toggle)));
        }

        if(cdata.has(collision_user_data_type::booster)) {
            begin_ball(a, static_cast<booster*>(cdata.get_ptr(collision_user_data_type::booster)));
        }

        if(cdata.has(collision_user_data_type::powerup)) {
            begin_ball(a, static_cast<powerup*>(cdata.get_ptr(collision_user_data_type::powerup)));
        }

        if(cdata.has(collision_user_data_type::flag)) {
            begin_ball(a, static_cast<flag*>(cdata.get_ptr(collision_user_data_type::flag)));
        }

        if(cdata.has(collision_user_data_type::portal)) {
            begin_ball(a, static_cast<portal*>(cdata.get_ptr(collision_user_data_type::portal)));
        }

        // check tiles
        if(cdata.has(collision_user_data_type::speed_yellow)) {
            a->on_tile_speed_counter++;
        }
        if(a->type == ball_type::red) {
            if(cdata.has(collision_user_data_type::speed_red)) {
                a->on_tile_speed_counter++;
            }
            if(cdata.has(collision_user_data_type::endzone_red)) {
                a->on_tile_endzone_counter++;
            }
        }

        if(a->type == ball_type::blue) {
            if(cdata.has(collision_user_data_type::speed_blue)) {
                a->on_tile_speed_counter++;
            }
            if(cdata.has(collision_user_data_type::endzone_blue)) {
                a->on_tile_endzone_counter++;
            }
        }


    }
}

void contact_listener::EndContact(b2Contact* contact)
{
    b2WorldManifold manifold;
    contact->GetWorldManifold(&manifold);

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

        ball * a = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        );

        /*
        if(cdata.has(collision_user_data_type::chain)) {
            end_ball(a,
                static_cast<chain*>(cdata.get_ptr(collision_user_data_type::chain)),
                manifold
            );
        }*/

        if(cdata.has(collision_user_data_type::toggle)) {
            end_ball(a, static_cast<toggle*>(cdata.get_ptr(collision_user_data_type::toggle)));
        }

        if(cdata.has(collision_user_data_type::portal)) {
            end_ball(a, static_cast<portal*>(cdata.get_ptr(collision_user_data_type::portal)));
        }

        if(cdata.has(collision_user_data_type::gate)) {
            end_ball(a, static_cast<gate*>(cdata.get_ptr(collision_user_data_type::gate)));
        }

        if(cdata.has(collision_user_data_type::speed_yellow)) {
            a->on_tile_speed_counter--;
        }
        if(a->type == ball_type::red) {
            if(cdata.has(collision_user_data_type::speed_red)) {
                a->on_tile_speed_counter--;
            }
            if(cdata.has(collision_user_data_type::endzone_red)) {
                a->on_tile_endzone_counter--;
            }
        }

        if(a->type == ball_type::blue) {
            if(cdata.has(collision_user_data_type::speed_blue)) {
                a->on_tile_speed_counter--;
            }
            if(cdata.has(collision_user_data_type::endzone_blue)) {
                a->on_tile_endzone_counter--;
            }
        }
    }
}

void contact_listener::begin_ball(ball* a, ball* b, b2WorldManifold manifold)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin ball");

    const b2Vec2 bpos(a->body->GetPosition());
    const b2Vec2 cpos(manifold.points[0]);

    if(cpos.y > bpos.y) {
        a->jumps_since_land_counter=0;
    }

    if(! same_color(a->type, b->type)) {
        bool disable_flag_check_a = a->grab_invincibility_counter > 0;
        bool disable_flag_check_b = b->grab_invincibility_counter > 0;

        if(! a->powerups.empty() || ! b->powerups.empty()) {
            if(a->has_powerup(powerup_type::rollingbomb)) {
                disable_flag_check_a = true;
                a->rb_explode();
            }

            if(b->has_powerup(powerup_type::rollingbomb)) {
                disable_flag_check_b = true;
                b->rb_explode();
            }

            if(a->has_powerup(powerup_type::tagpro)) {
                b->pop();
                a->player_ptr->stats.tags++;
                b->player_ptr->stats.popped_tagged++;
            }
            if(b->has_powerup(powerup_type::tagpro)) {
                a->pop();
                b->player_ptr->stats.tags++;
                a->player_ptr->stats.popped_tagged++;
            }
        }

        if(! disable_flag_check_a && ! a->flags.empty()) {
            if(a->has_flag(inv_corresponding_color<flag_type>(a->type))) {
                a->pop();
                b->player_ptr->stats.flag_returns++;
            } else if(a->has_flag(flag_type::neutral)) {
                b->take_flags(a, flag_type::neutral);
                a->pop();
            }
        }

        if(! disable_flag_check_b && ! b->flags.empty()) {
            if(b->has_flag(inv_corresponding_color<flag_type>(b->type))) {
                b->pop();
                a->player_ptr->stats.flag_returns++;
            } else if(b->has_flag(flag_type::neutral)) {
                a->take_flags(b, flag_type::neutral);
                b->pop();
            }
        }
    }
}

void contact_listener::begin_ball(ball* a, chain* b, b2WorldManifold manifold)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin ball/chain");
    const b2Vec2 bpos(a->body->GetPosition());
    const b2Vec2 cpos(manifold.points[0]);

    if(cpos.y > bpos.y) {
        a->jumps_since_land_counter=0;
    }
}

void contact_listener::begin_ball(ball* a, spike* b)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin spike");

    if(a) {
        a->pop();
        a->player_ptr->stats.popped_spikes++;
    }
}

void contact_listener::begin_ball(ball* a, gate* b)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin gate");

    if(b) {
        a->in_gate_ptrs.insert(b);
    }
}

void contact_listener::begin_ball(ball* a, bomb* b, b2WorldManifold manifold)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin bomb");

    const b2Vec2 bpos(a->body->GetPosition());
    const b2Vec2 cpos(manifold.points[0]);

    if(cpos.y > bpos.y) {
        a->jumps_since_land_counter=0;
    }

    if(b) {
        b->explode();
        a->player_ptr->stats.bombs_hit++;
    }
}

void contact_listener::begin_ball(ball* a, toggle* b)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin toggle");

    if(b) b->step_on(a);
}

void contact_listener::begin_ball(ball* a, booster* b)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin booster");

    if(b) b->step_on(a);
}

void contact_listener::begin_ball(ball* a, powerup* b)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin powerup");

    if(b) b->step_on(a);
}

void contact_listener::begin_ball(ball* a, flag* b)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin flag");

    if(b) b->step_on(a);
}

void contact_listener::begin_ball(ball* a, portal* b)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: begin portal");

    if(b) b->step_on(a);
}

/*
void contact_listener::end_ball(ball* a, chain* b, b2WorldManifold manifold)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: end ball/chain");

    const b2Vec2 bpos(a->body->GetPosition());
    const b2Vec2 cpos(manifold.points[0]);
}
*/

void contact_listener::end_ball(ball* a, toggle* b)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: left toggle");

    if(b) b->step_off(a);
}

void contact_listener::end_ball(ball* a, portal* b)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: portal");

    if(b) b->step_off(a);
}

void contact_listener::end_ball(ball* a, gate* b)
{
    if(VERBOSE_CONTACT) spdlog::get("game")->debug("contact: left gate");

    if(a) a->in_gate_ptrs.erase(b);
}
