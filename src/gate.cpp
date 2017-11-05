#include "gate.hpp"
#include <json/json.hpp>

thread_local std::size_t gate::id_counter = 0;

gate::gate(){}
gate::gate(
    const polygon poly,
    const gate_type type
)
: id(id_counter++)
, poly(poly)
, type(type)
, current(type)
, body(nullptr)
{}

void gate::mark_on(ball* b)
{
    if(b->type == ball_type::blue) --red_minus_blue;
    else if(b->type == ball_type::red) ++red_minus_blue;
    else spdlog::get("game")->error("ball_type neither red nor blue");

    if(red_minus_blue == 0) { current = type; return; }
    current = red_minus_blue < 0 ? gate_type::blue : gate_type::red;
}

void gate::mark_off(ball* b)
{
    if(b->type == ball_type::blue) ++red_minus_blue;
    else if(b->type == ball_type::red) --red_minus_blue;
    else spdlog::get("game")->error("ball_type neither red nor blue");

    if(red_minus_blue == 0) { current = type; return; }
    current = red_minus_blue < 0 ? gate_type::blue : gate_type::red;
}

void gate::kill_if_other(std::unique_ptr<ball>& b) {
    if(current == gate_type::off) return;
    if(!same_color(current,b->type)) b->pop();
}

void gate::add_to_world(b2World* world) {
    const b2Vec2 center = poly.get_center();
    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(center.x, center.y);
    bdef.angle = 0.0f;
    body = world->CreateBody(&bdef);

    const std::array<b2Vec2, 3> vertices = poly.get_vertices();

    b2PolygonShape bshape;
    bshape.Set(vertices.data(), vertices.size());

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    fdef.density = 1;
    body->CreateFixture(&fdef);
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(collision_user_data_type::gate, this));
    body->SetUserData(static_cast<void*>(col_data.get()));
}

void to_json(nlohmann::json& j, const gate& p)
{
    j = nlohmann::json{{"poly", p.poly}, {"type", to_string(p.type)}};
}

void from_json(const nlohmann::json& j, gate& p)
{
    p = gate(
        j.at("poly").get<polygon>(),
        gate_type_from_string(j.at("type").get<std::string>())
    );
}

