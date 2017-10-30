#include "toggle_tag.hpp"

void toggle_tag::step_on(ball* o)
{
    switch(type) {
        case toggle_tag_type::bomb: static_cast<bomb*>(ptr)->explode(); break;
        case toggle_tag_type::gate: static_cast<gate*>(ptr)->mark_on(o); break;
    }
}

void toggle_tag::step_off(ball* o)
{
    switch(type) {
        case toggle_tag_type::bomb: break;
        case toggle_tag_type::gate: static_cast<gate*>(ptr)->mark_off(o); break;
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

