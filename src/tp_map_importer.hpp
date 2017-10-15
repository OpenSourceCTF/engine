#ifndef ML_TP_MAP_IMPORTER_HPP
#define ML_TP_MAP_IMPORTER_HPP

#include <cstdint>
#include <string>
#include "map.hpp"

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
    booster_all,
    booster_red,
    booster_blue,
    gate,
    portal,
    flag_neutral,
    flag_red,
    flag_blue
};

struct tp_map_importer
{
    map& m;

    tp_map_importer(map& m)
    : m(m)
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
};

#endif

