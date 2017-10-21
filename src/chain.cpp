#include "chain.hpp"

void chain::add_vertex(const coord a) {
    vertices.push_back(a);
}

void to_json(nlohmann::json& j, const chain& ch) {
    j = nlohmann::json {
        {"vertices", ch.vertices}
    };
}

void from_json(const nlohmann::json& j, chain& ch) {
    ch.vertices = j.at("vertices").get<std::vector<coord>>();
}
