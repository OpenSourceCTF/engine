#ifndef ML_MAP_HPP
#define ML_MAP_HPP

#include <Box2D/Box2D.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <map>
#include <random>
#include <cmath>
#include <memory>
#include <algorithm>

#include <json.hpp>

#include "lodepng.h"
#include "map_type.hpp"
#include "polygon.hpp"
#include "color.hpp"
#include "wall.hpp"
#include "tile.hpp"
#include "util.hpp"
#include "portal.hpp"
#include "toggle.hpp"
#include "toggle_tag_type.hpp"
#include "spawn.hpp"
#include "bomb.hpp"
#include "spike.hpp"
#include "powerup.hpp"
#include "booster.hpp"
#include "gate.hpp"
#include "flag.hpp"
#include "polygon.hpp"
#include "ball.hpp"
#include "ball_powerup.hpp"
#include "contact_listener.hpp"
#include "random_util.hpp"
#include "chain.hpp"

struct toggle;
struct ball;
struct gate;
struct booster;
struct portal;
struct wall;
struct spike;
struct bomb;
struct powerup;
struct flag;

struct map
{
    bool is_loaded;

    map_type type;
    std::string name;
    std::string author;
    std::uint32_t version;
    std::uint32_t width;
    std::uint32_t height;

    std::vector<std::unique_ptr<wall>>    walls;
    std::vector<std::unique_ptr<tile>>    tiles;
    
    std::vector<std::unique_ptr<portal>>  portals;
    std::vector<std::unique_ptr<toggle>>  toggles;
    std::vector<std::unique_ptr<spawn>>   spawns;
    std::vector<std::unique_ptr<bomb>>    bombs;
    std::vector<std::unique_ptr<spike>>   spikes;
    std::vector<std::unique_ptr<booster>> boosters;
    std::vector<std::unique_ptr<gate>>    gates;
    std::vector<std::unique_ptr<flag>>    flags;
    std::vector<std::unique_ptr<powerup>> powerups;
    std::vector<std::unique_ptr<ball>>    balls;
    std::vector<std::unique_ptr<chain>>   chains;

    map();
};

void to_json(nlohmann::json& j, const map& p);
void from_json(const nlohmann::json& j, map& p);

#endif
