#include "contact_filter.hpp"

const collision_user_data unknown_data;

bool contact_filter::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
{
    const collision_user_data* user_data_a = static_cast<collision_user_data*>(
        fixtureA->GetBody()->GetUserData()
    );
    const collision_user_data* user_data_b = static_cast<collision_user_data*>(
        fixtureB->GetBody()->GetUserData()
    );

    const cslot cdata({
        (user_data_a) ? *user_data_a : unknown_data,
        (user_data_b) ? *user_data_b : unknown_data
    });

    if(cdata.has(collision_user_data_type::ball)) {
        ball * a = static_cast<ball*>(
            cdata.get_ptr(collision_user_data_type::ball)
        ); if(! a->is_alive) return false;

        if(cdata.has_both(collision_user_data_type::ball)) {
            ball * b = static_cast<ball*>(
                cdata.get_ptr_alt(collision_user_data_type::ball)
            ); if(! b->is_alive) return false;
        }

        if(cdata.has(collision_user_data_type::bomb)) {
            bomb * b = static_cast<bomb*>(
                cdata.get_ptr(collision_user_data_type::bomb)
            ); if(! b->is_alive) return false;
        }

        if(cdata.has(collision_user_data_type::booster)) {
            booster * b = static_cast<booster*>(
                cdata.get_ptr(collision_user_data_type::booster)
            ); if(! b->is_alive) return false;
        }

        if(cdata.has(collision_user_data_type::powerup)) {
            powerup * b = static_cast<powerup*>(
                cdata.get_ptr(collision_user_data_type::powerup)
            ); if(! b->is_alive) return false;
        }

        if(cdata.has(collision_user_data_type::flag)) {
            flag * b = static_cast<flag*>(
                cdata.get_ptr(collision_user_data_type::flag)
            ); if(! b->is_alive) return false;
        }

        if(cdata.has(collision_user_data_type::portal)) {
            portal * b = static_cast<portal*>(
                cdata.get_ptr(collision_user_data_type::portal)
            ); if(! b->is_alive) return false;
        }
    }

    return true;
}
