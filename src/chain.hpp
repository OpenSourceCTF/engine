#ifndef ML_CHAIN_HPP
#define ML_CHAIN_HPP

#include <vector>
#include "coord.hpp"

struct chain {
    std::vector<coord> vertices;

    chain() : vertices(0) {}

    chain(const std::vector<coord>& v) : vertices(v) {
        //sort CC/CCW ?
    }

    void add_vertex(const coord a);
};

void to_json(nlohmann::json& j, const chain& p);
void from_json(const nlohmann::json& j, chain& p);

#endif
