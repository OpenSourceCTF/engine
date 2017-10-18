#include "toggle_tag.hpp"

void toggle_tag::step_on(map& m, ball* o)
{
    switch(type) {
        case toggle_tag_type::bomb: m.bombs[id].explode(o);  break;
        case toggle_tag_type::gate: m.gates[id].mark_on(o); break;
    }
}

void toggle_tag::step_off(map& m, ball* o)
{
    switch(type) {
        case toggle_tag_type::bomb: break;
        case toggle_tag_type::gate: m.gates[id].mark_off(o); break;
    }
}

void to_json(nlohmann::json& j, const toggle_tag& p)
{
    j = nlohmann::json{{"id", p.id}, {"type", to_string(p.type)}};
}

void from_json(const nlohmann::json& j, toggle_tag& p)
{
    p.id   = j.at("id").get<int>();
    p.type = toggle_tag_type_from_string(j.at("type").get<std::string>());
}

