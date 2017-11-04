#ifndef ML_TP_MAP_IMPORTER_HPP
#define ML_TP_MAP_IMPORTER_HPP

#include <cstdint>
#include <string>
#include "map.hpp"
#include "powerup_type.hpp"

struct tp_map_importer
{
    map& m;

    tp_map_importer(map& m);

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

