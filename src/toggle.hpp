#ifndef ML_TOGGLE_HPP
#define ML_TOGGLE_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include "libs/json.hpp"
#include "map.hpp"
#include "toggle_tag_type.hpp"
#include "toggle_tag.hpp"

struct map;
struct toggle_tag;

struct toggle
{
    float x;
    float y;
    std::uint32_t timer;
    std::vector<toggle_tag> tags;

    toggle(){}
    toggle(
        const float x,
        const float y,
        const std::uint32_t timer,
        const std::vector<toggle_tag> tags
    )
    : x(x)
    , y(y)
    , timer(timer)
    , tags(tags)
    {}

    void step_on(map& m);
    void step_off(map& m);
};

void to_json(nlohmann::json& j, const toggle& p);
void from_json(const nlohmann::json& j, toggle& p);

#endif

