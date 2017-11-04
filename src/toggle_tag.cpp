#include "toggle_tag.hpp"

toggle_tag::toggle_tag(){}
toggle_tag::toggle_tag(
    const std::uint32_t id,
    const toggle_tag_type type
)
: id(id)
, ptr(nullptr)
, type(type)
{}

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
    p = toggle_tag(
        j.at("id").get<int>(),
        toggle_tag_type_from_string(j.at("type").get<std::string>())
    );
}

