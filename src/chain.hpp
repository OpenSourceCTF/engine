#ifndef ML_CHAIN_HPP
#define ML_CHAIN_HPP

#include <vector>
#include <Box2D/Box2D.h>
#include "collision_user_data.hpp"
#include "coord.hpp"
#include "util.hpp"

struct collision_user_data;

struct chain
{
    std::vector<coord> vertices;
    b2Body* body;
    std::shared_ptr<collision_user_data> col_data;

    chain();
    chain(const std::vector<coord> & vertices);
    ~chain();

    void add_vertex(const float x, const float y);
    void add_to_world(b2World * world);
};

void to_json(nlohmann::json& j, const chain& p);
void from_json(const nlohmann::json& j, chain& p);

#endif
