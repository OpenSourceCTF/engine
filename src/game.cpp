#include "game.hpp"

void to_json(nlohmann::json& j, const game& p)
{
    // todo
    // we want score and time remaining here as well
    j = nlohmann::json{
        {"port",         p.port},
        {"max_points",   p.max_points},
        {"max_length",   p.max_length},
        {"map_name",     p.m->name},
        {"map_author",   p.m->author},
        {"player_count", p.m->balls.size()}
    };
}
