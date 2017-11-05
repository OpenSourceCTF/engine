#include "gate_type.hpp"
#include <json/json.hpp>

std::string to_string(const gate_type m)
{
    switch(m) {
        case gate_type::off:   return "off";   break;
        case gate_type::green: return "green"; break;
        case gate_type::blue:  return "blue";  break;
        case gate_type::red:   return "red";   break;
    }
}

gate_type gate_type_from_string(const std::string & m)
{
    if(m == "off")   { return gate_type::off; }
    if(m == "green") { return gate_type::green; }
    if(m == "blue")  { return gate_type::blue; }
    if(m == "red")   { return gate_type::red; }

    spdlog::get("game")->error("gate_type ", m, " not enumerated in from_string");
    return gate_type::off; // for warning suppression mainly
}
