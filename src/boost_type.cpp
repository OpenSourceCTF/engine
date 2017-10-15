#include "boost_type.hpp"

std::string to_string(const boost_type m)
{
    switch(m) {
        case boost_type::all:  return "all";  break;
        case boost_type::blue: return "blue"; break;
        case boost_type::red:  return "red";  break;
        default:
            std::cerr << "error: boost_type not enumerated in to_string" << std::endl;
            return "";
    }
}

boost_type boost_type_from_string(const std::string & m)
{
    if(m == "all")  { return boost_type::all; }
    if(m == "blue") { return boost_type::blue; }
    if(m == "red")  { return boost_type::red; }

    std::cerr << "error: boost_type " << m << " not enumerated in from_string" << std::endl;
    return boost_type::all; // for warning suppression mainly
}
