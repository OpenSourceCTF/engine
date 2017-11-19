#ifndef ML_GRAVWELL_HPP
#define ML_GRAVWELL_HPP

#include <memory>
#include <Box2D/Box2D.h>
#include <json/json_fwd.hpp>
#include "collision_user_data.hpp"
#include "settings.hpp"
#include "game_accessor.hpp"
#include "gravwell_aabb_callback.hpp"

struct collision_user_data;

struct gravwell
{
    game_accessor game;
    float x;
    float y;
    float r;
    float f;
    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;

    gravwell();
    gravwell(
        const float x,
        const float y,
        const float r,
        const float f
    );
    ~gravwell();

    void add_to_world(b2World * world);
    void suck(b2World* world);
};

void to_json(nlohmann::json& j, const gravwell& p);
void from_json(const nlohmann::json& j, gravwell& p);


#endif

