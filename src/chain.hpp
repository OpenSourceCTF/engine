#ifndef ML_CHAIN_HPP
#define ML_CHAIN_HPP

#include <vector>
#include <Box2D/Box2D.h>
#include "coord.hpp"

struct chain {
    std::vector<std::unique_ptr<coord>> vertices;

    chain() : vertices(0) {}

    void add_vertex(const float x, const float y);
};

void to_json(nlohmann::json& j, const chain& p);
void from_json(const nlohmann::json& j, chain& p);

#endif
