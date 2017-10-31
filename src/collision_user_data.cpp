#include "collision_user_data.hpp"

collision_user_data::collision_user_data()
: ptr(nullptr)
, type(collision_user_data_type::unknown)  
{}

collision_user_data::collision_user_data(collision_user_data_type type, void* ptr)
: ptr(ptr)
, type(type)
{}
