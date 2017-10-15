#ifndef ML_GATE_HPP
#define ML_GATE_HPP

#include <string>
#include <iostream>

#include "libs/json.hpp"
#include "polygon.hpp"

#include "gate_type.hpp"


struct gate
{
    polygon poly;
    gate_type type;

    gate_type current;

    gate(){}
    gate(
        const polygon poly,
        const gate_type type
    )
    : poly(poly)
    , type(type)
    , current(type)
    {}

    void mark_on();
    void mark_off();
};

void to_json(nlohmann::json& j, const gate& p);
void from_json(const nlohmann::json& j, gate& p);

#endif
