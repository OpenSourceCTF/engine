#include "cslot.hpp"

cslot::cslot(const std::array<collision_user_data, 2> held)
: held(held)
{}

bool cslot::has(const collision_user_data_type type) const
{
    return held[0].type == type || held[1].type == type;
}

bool cslot::has_both(const collision_user_data_type type) const
{
    return held[0].type == type && held[1].type == type;
}

// only call this if you've verified with has
// otherwise (very) unsafe
void* cslot::get_ptr(const collision_user_data_type type) const
{
    return held[0].type == type ? held[0].ptr : held[1].ptr;
}

// use this in conjunction with above for 2 of same type
void* cslot::get_ptr_alt(const collision_user_data_type type) const
{
    return held[1].type == type ? held[1].ptr : held[0].ptr;
}
