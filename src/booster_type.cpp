#include "booster_type.hpp"
#include <json/json.hpp>

std::string to_string(const booster_type m)
{
    switch(m) {
        case booster_type::all:  return "all";  break;
        case booster_type::blue: return "blue"; break;
        case booster_type::red:  return "red";  break;
    }
}

booster_type booster_type_from_string(const std::string & m)
{
    if(m == "all")  { return booster_type::all; }
    if(m == "blue") { return booster_type::blue; }
    if(m == "red")  { return booster_type::red; }

    spdlog::get("game")->error("booster_type ", m, " not enumerated in from_string");
    return booster_type::all; // for warning suppression mainly
}
