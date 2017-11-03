#include "gate.hpp"

void gate::mark_on(ball* b)
{
    // todo -- this needs work
    switch(type) {
        case gate_type::off:  current = gate_type::on;  break;
        case gate_type::on:   current = gate_type::off; break;
        case gate_type::red:
        case gate_type::blue:
            if(! same_color(type, b->type)) current = gate_type::off;
            else {} // todo - we need to track amount of people on/off toggles for this to work 
            break;
    }
}

void gate::mark_off(ball* b)
{
    current = type;
}

void to_json(nlohmann::json& j, const gate& p)
{
    j = nlohmann::json{{"poly", p.poly}, {"type", to_string(p.type)}};
}

void from_json(const nlohmann::json& j, gate& p)
{
    p = gate(
        j.at("poly").get<polygon>(),
        gate_type_from_string(j.at("type").get<std::string>())
    );
}

