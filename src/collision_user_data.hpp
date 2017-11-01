#ifndef ML_COLLISION_USER_DATA_HPP
#define ML_COLLISION_USER_DATA_HPP

#include "collision_user_data_type.hpp"

struct collision_user_data
{
    void * ptr;
    collision_user_data_type type;

    collision_user_data();
    collision_user_data(collision_user_data_type type, void* ptr);
};

#endif

