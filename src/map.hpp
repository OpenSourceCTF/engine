#ifndef ML_MAP_HPP
#define ML_MAP_HPP

#ifndef DISABLE_RENDER
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#endif

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

#include "libs/lodepng.h"
#include "libs/json.hpp"

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
#include "contact_listener.hpp"


struct map
{
    bool is_loaded;

    map_type type;
    std::string name;
    std::string author;
    std::uint32_t version;
    std::uint32_t width;
    std::uint32_t height;

    std::vector<wall>    walls;
    std::vector<tile>    tiles;
    
    std::vector<portal>  portals;
    std::vector<toggle>  toggles;
    std::vector<spawn>   spawns;
    std::vector<bomb>    bombs;
    std::vector<spike>   spikes;
    std::vector<powerup> powerups;
    std::vector<booster> boosters;
    std::vector<gate>    gates;
    std::vector<flag>    flags;
    std::vector<ball>    balls;

    map()
    : is_loaded(false)
    {}

    b2World* init_world();
    int render();
};

void to_json(nlohmann::json& j, const map& p);
void from_json(const nlohmann::json& j, map& p);

#endif