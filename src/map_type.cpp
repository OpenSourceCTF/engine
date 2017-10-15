#include "map_type.hpp"

std::string to_string(const map_type m)
{
    switch(m) {
        case map_type::normal:          return "normal";          break;
        case map_type::none_gravityCTF: return "none_gravityCTF"; break;
        default:
            std::cerr << "error: tile_type not enumerated in to_string" << std::endl;
            return "";
    }
}

map_type map_type_from_string(const std::string & m)
{
    if(m == "normal")          { return map_type::normal; }
    if(m == "none_gravityCTF") { return map_type::none_gravityCTF; }

    std::cerr << "error: map_type " << m << " not enumerated in from_string" << std::endl;
    return map_type::normal; // for warning suppression mainly
}
