#ifndef ML_CSLOT_HPP
#define ML_CSLOT_HPP

#include <array>
#include "collision_user_data.hpp"


// this holds two collision user datas together
// representing a contact between two things
struct cslot
{
    std::array<collision_user_data, 2> held;

    cslot(const std::array<collision_user_data, 2> held);

    bool has(const collision_user_data_type type) const;
    bool has_both(const collision_user_data_type type) const;
    void* get_ptr(const collision_user_data_type type) const;
    void* get_ptr_alt(const collision_user_data_type type) const;
};

#endif

