#include "flag_type.hpp"

std::string to_string(const flag_type m)
{
    switch(m) {
        case flag_type::neutral: return "neutral"; break;
        case flag_type::blue:    return "blue";    break;
        case flag_type::red:     return "red";     break;
    }
}


flag_type flag_type_from_string(const std::string & m)
{
    if(m == "neutral") { return flag_type::neutral; }
    if(m == "blue")    { return flag_type::blue; }
    if(m == "red")     { return flag_type::red; }

    spdlog::get("game")->error("flag_type ", m, " not enumerated in from_string");
    return flag_type::neutral; // for warning suppression mainly
}

