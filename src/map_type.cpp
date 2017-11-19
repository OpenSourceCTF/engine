#include "map_type.hpp"
#include <json/json.hpp>

std::string to_string(const map_type m)
{
    switch(m) {
        case map_type::normal:          return "normal";          break;
        case map_type::none_gravityCTF: return "none_gravityCTF"; break;
        case map_type::gravity:         return "gravity";         break;
        case map_type::gravityCTF:      return "gravityCTF";      break;
    }
}

map_type map_type_from_string(const std::string & m)
{
    if(m == "normal")          { return map_type::normal; }
    if(m == "none_gravityCTF") { return map_type::none_gravityCTF; }
    if(m == "gravity")         { return map_type::gravity; }
    if(m == "gravityCTF")      { return map_type::gravityCTF; }

    spdlog::get("game")->error("map_type ", m, " not enumerated in from_string");
    return map_type::normal; // for warning suppression mainly
}
