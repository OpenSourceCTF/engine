#include "toggle.hpp"


void toggle::step_on(map& m)
{
    std::cout << "toggle stepped on" << std::endl;

    for(auto & o : tags) {
        o.step_on(m);
    }
}

void toggle::step_off(map& m)
{
    std::cout << "toggle stepped off" << std::endl;

    for(auto & o : tags) {
        o.step_off(m);
    }
}

void to_json(nlohmann::json& j, const toggle& p)
{
    j = nlohmann::json{
        {"x", p.x},
        {"y", p.y},
        {"timer", p.timer},
        {"tags", p.tags}
    };
}

void from_json(const nlohmann::json& j, toggle& p)
{
    p.x     = j.at("x").get<float>();
    p.y     = j.at("y").get<float>();
    p.timer = j.at("timer").get<int>();
    p.tags  = j.at("tags").get<std::vector<toggle_tag>>();
}

