#include "chain.hpp"

chain::~chain()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
    }
}

void chain::add_vertex(const float x, const float y) {
    vertices.emplace_back(x,y);
}

void chain::add_to_world(b2World * world)
{
    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(0.0f,0.0f);
    bdef.angle = 0.0f;

    body = world->CreateBody(&bdef);


    std::vector<b2Vec2> vs(vertices.size());
    int i = 0;
    for(auto& v : vertices) {
        vs[i].Set(v.x,v.y);
        ++i;
    }
    b2ChainShape ch;
    ch.CreateChain(vs.data(), vertices.size());

    b2FixtureDef fdef;
    fdef.shape = &ch;
    fdef.density = 1;
    body->CreateFixture(&fdef);
}

void to_json(nlohmann::json& j, const chain& p)
{
    j = nlohmann::json {
        {"vertices", p.vertices}
    };
}

void from_json(const nlohmann::json& j, chain& p)
{
    p = chain(j.at("vertices").get<std::vector<coord>>());
}
