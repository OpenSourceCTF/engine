#include "spawn_type.hpp"

std::string to_string(const spawn_type m)
{
    switch(m) {
        case spawn_type::blue: return "blue"; break;
        case spawn_type::red:  return "red";  break;
        default:
            std::cerr << "error: spawn_type not enumerated in to_string" << std::endl;
            return "";
    }
}

spawn_type spawn_type_from_string(const std::string & m)
{
    if(m == "red")  { return spawn_type::red; }
    if(m == "blue") { return spawn_type::blue; }

    std::cerr << "error: spawn_type " << m << " not enumerated in from_string" << std::endl;
    return spawn_type::red; // for warning suppression mainly
}
