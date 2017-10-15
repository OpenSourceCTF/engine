#include "booster_type.hpp"

std::string to_string(const booster_type m)
{
    switch(m) {
        case booster_type::all:  return "all";  break;
        case booster_type::blue: return "blue"; break;
        case booster_type::red:  return "red";  break;
        default:
            std::cerr << "error: booster_type not enumerated in to_string" << std::endl;
            return "";
    }
}

booster_type booster_type_from_string(const std::string & m)
{
    if(m == "all")  { return booster_type::all; }
    if(m == "blue") { return booster_type::blue; }
    if(m == "red")  { return booster_type::red; }

    std::cerr << "error: booster_type " << m << " not enumerated in from_string" << std::endl;
    return booster_type::all; // for warning suppression mainly
}
