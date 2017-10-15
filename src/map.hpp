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
#include "boost.hpp"
#include "gate.hpp"
#include "flag.hpp"
#include "polygon.hpp"
#include "ball.hpp"
#include "contact_listener.hpp"


// the types of tiles from tagpro .png map
enum class tp_tile_type
{
    background,
    tile,
    speed_red,
    speed_blue,
    endzone_red,
    endzone_blue,
    wall,
    wall_tl,
    wall_tr,
    wall_bl,
    wall_br,
    bomb,
    spike,
    powerup,
    button,
    boost_all,
    boost_red,
    boost_blue,
    gate,
    portal,
    flag_neutral,
    flag_red,
    flag_blue
};

struct toggle;

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
    std::vector<boost>   boosts;
    std::vector<gate>    gates;
    std::vector<flag>    flags;
    std::vector<ball>    balls;

    map()
    : is_loaded(false)
    {}

    int tp_import(
        const std::string & json_src,
        const std::string & png_src
    ); 

    // reference toggle and add item to it if it matches position
    // this is only for tp imports
    void tp_toggle_ref(
        const std::uint32_t x,
        const std::uint32_t y,
        const std::size_t id,
        const toggle_tag_type type
    ); 

    // todo: this should catch exceptions (esp w/ stoi)
    int tp_import_json(const std::string & src);
    int tp_import_png(const std::string & src);
    b2World* init_world();
    int render();
};

void to_json(nlohmann::json& j, const map& p);
void from_json(const nlohmann::json& j, map& p);

#endif
