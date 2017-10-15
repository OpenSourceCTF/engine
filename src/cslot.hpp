#ifndef ML_CSLOT_HPP
#define ML_CSLOT_HPP

#include <array>
#include "collision_user_data.hpp"

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

#endif

