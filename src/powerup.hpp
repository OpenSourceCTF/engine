#ifndef ML_POWERUP_HPP
#define ML_POWERUP_HPP

#include <memory>
#include <vector>
#include <string>
#include <Box2D/Box2D.h>
#include <json.hpp>
#include "collision_user_data.hpp"
#include "settings.hpp"
#include "random_util.hpp"
#include "ball.hpp"
#include "powerup_type.hpp"

struct collision_user_data;

struct powerup
{
    float x;
    float y;
    b2Body * body;
    std::shared_ptr<collision_user_data> col_data;
    bool is_alive;
    int respawn_counter;
    std::vector<powerup_type> possible_types;
    powerup_type type;

    powerup();
    powerup(
        const float x,
        const float y,
        const std::vector<powerup_type> possible_types
    );

    void add_to_world(b2World * world);
    void step_on(ball* m);
    powerup_type get_random_type();
};

void to_json(nlohmann::json& j, const powerup& p);
void from_json(const nlohmann::json& j, powerup& p);

#endif

