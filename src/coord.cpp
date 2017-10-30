#include "coord.hpp"

void to_json(nlohmann::json& j, const coord& c) {
    j = nlohmann::json {
        {"x", c.x}, {"y", c.y}
    };
}

void from_json(const nlohmann::json& j, coord& c) {
    c.x = j.at("x").get<float>();
    c.y = j.at("y").get<float>();
}
