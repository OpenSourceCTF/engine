#include "toggle_tag_type.hpp"

std::string to_string(const toggle_tag_type m)
{
    switch(m) {
        case toggle_tag_type::bomb: return "bomb"; break;
        case toggle_tag_type::gate: return "gate"; break;
        default:
            std::cerr << "error: toggle_tag_type not enumerated in to_string" << std::endl;
            return "";
    }
}

toggle_tag_type toggle_tag_type_from_string(const std::string & m)
{
    if(m == "bomb") { return toggle_tag_type::bomb; }
    if(m == "gate") { return toggle_tag_type::gate; }

    std::cerr << "error: toggle_tag_type " << m << " not enumerated in from_string" << std::endl;
    return toggle_tag_type::bomb; // for warning suppression mainly
}

