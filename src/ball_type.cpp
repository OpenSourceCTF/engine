#include "ball_type.hpp"

std::string to_string(const ball_type m)
{
    switch(m) {
        case ball_type::red:  return "red"; break;
        case ball_type::blue: return "blue"; break;
        default:
            std::cerr << "error: ball_type not enumerated in to_string" << std::endl;
            return "";
    }
}

ball_type ball_type_from_string(const std::string & m)
{
    if(m == "red")  { return ball_type::red; }
    if(m == "blue") { return ball_type::blue; }

    std::cerr << "error: ball_type " << m << " not enumerated in from_string" << std::endl;
    return ball_type::red; // for warning suppression mainly
}

