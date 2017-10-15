#include "gate.hpp"

void gate::mark_on()
{
    // todo -- this needs work
    switch(type) {
        case gate_type::off:  current = gate_type::on;  break;
        case gate_type::on:   current = gate_type::off; break;
    }
}

void gate::mark_off()
{
    current = type;
}

void from_json(const nlohmann::json& j, gate& p)
{
    p.poly = j.at("poly").get<polygon>();
    p.type = gate_type_from_string(j.at("type").get<std::string>());
}

void to_json(nlohmann::json& j, const gate& p)
{
    j = nlohmann::json{{"poly", p.poly}, {"type", to_string(p.type)}};
}
