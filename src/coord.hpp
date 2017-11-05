#ifndef ML_COORD_HPP
#define ML_COORD_HPP

#include <json/json.hpp>

struct coord
{

    float x,y;
    
    coord();
    
    coord(
        const float x,
        const float y
    );
};

void to_json(nlohmann::json& j, const coord& p);
void from_json(const nlohmann::json& j, coord& p);

#endif
