#ifndef ML_PMS_MAP_IMPORTER_HPP
#define ML_PMS_MAP_IMPORTER_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include "map.hpp"
#include "powerup_type.hpp"
#include "pms_format.hpp"

struct pms_map_importer
{
    map& m;

    pms_map_importer(map& m);

    int import(
        const std::string & src,
        const double scale_size
    );
};

#endif


