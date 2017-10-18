#ifndef ML_TOGGLE_TAG_HPP
#define ML_TOGGLE_TAG_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include "libs/json.hpp"
#include "toggle_tag_type.hpp"
#include "map.hpp"
#include "ball.hpp"
#include "util.hpp"

struct map;

// id is just the offset in the list of things
// so id=5 would be map.bombs[4]
// should we add action?
// then we could have stuff like spawn bombs or other weird crap
struct toggle_tag
{
    std::uint32_t id;
    toggle_tag_type type;

    toggle_tag(){}
    toggle_tag(
        const std::uint32_t id,
        const toggle_tag_type type
    )
    : id(id)
    , type(type)
    {}

    void step_on(map& m, ball* o);
    void step_off(map& m, ball* o);

};


void to_json(nlohmann::json& j, const toggle_tag& p);
void from_json(const nlohmann::json& j, toggle_tag& p);

#endif

