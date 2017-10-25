#include "powerup_type.hpp"

std::string to_string(const powerup_type m)
{
    switch(m) {
        case powerup_type::jukejuice:   return "jukejuice"; break;
        case powerup_type::rollingbomb: return "rollingbomb"; break;
        case powerup_type::tagpro:      return "tagpro"; break;
        default:
            std::cerr << "error: powerup_type not enumerated in to_string" << std::endl;
            return "";
    }
}

powerup_type powerup_type_from_string(const std::string & m)
{
    if(m == "jukejuice")   { return powerup_type::jukejuice; }
    if(m == "rollingbomb") { return powerup_type::rollingbomb; }
    if(m == "tagpro")      { return powerup_type::tagpro; }

    std::cerr << "error: powerup_type " << m << " not enumerated in from_string" << std::endl;
    return powerup_type::jukejuice; // for warning suppression mainly
}

