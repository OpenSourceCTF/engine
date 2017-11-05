#include "tile_type.hpp"
#include <json/json.hpp>

std::string to_string(const tile_type m)
{
    switch(m) {
        case tile_type::background:   return "background";   break;
        case tile_type::normal:       return "normal";       break;
        case tile_type::speed_red:    return "speed_red";    break;
        case tile_type::speed_blue:   return "speed_blue";   break;
        case tile_type::endzone_red:  return "endzone_red";  break;
        case tile_type::endzone_blue: return "endzone_blue"; break;
    }
}

tile_type tile_type_from_string(const std::string & m)
{
    if(m == "background")   { return tile_type::background; }
    if(m == "normal")       { return tile_type::normal; }
    if(m == "speed_red")    { return tile_type::speed_red; }
    if(m == "speed_blue")   { return tile_type::speed_blue; }
    if(m == "endzone_red")  { return tile_type::endzone_red; }
    if(m == "endzone_blue") { return tile_type::endzone_blue; }

    spdlog::get("game")->error("tile_type ", m, " not enumerated in from_string");
    return tile_type::background; // for warning suppression mainly
}
