#include "chain.hpp"
#include "util.hpp"

void chain::add_vertex(const float x, const float y) {
}

void to_json(nlohmann::json& j, const chain& ch) {
    std::vector<coord> verts; for(auto & o : ch.vertices) verts.emplace_back(*o);
    j = nlohmann::json {
        {"vertices", verts}
    };
}

void from_json(const nlohmann::json& j, chain& ch) {
    ch.vertices = vec_to_uniq_ptr_vec(j.at("vertices").get<std::vector<coord>>());
}
