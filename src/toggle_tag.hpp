#ifndef ML_TOGGLE_TAG_HPP
#define ML_TOGGLE_TAG_HPP

#include <iostream>
#include <string>
#include <cstdint>
#include <json.hpp>
#include "toggle_tag_type.hpp"
#include "util.hpp"
#include "ball.hpp"
#include "gate.hpp"
#include "bomb.hpp"

// id is loaded first
// then ptr is set after map load
struct toggle_tag
{
    std::uint32_t id;
    void* ptr;
    toggle_tag_type type;

    toggle_tag();
    toggle_tag(
        const std::uint32_t id,
        const toggle_tag_type type
    );

    void step_on(ball* o);
    void step_off(ball* o);

};


void to_json(nlohmann::json& j, const toggle_tag& p);
void from_json(const nlohmann::json& j, toggle_tag& p);

#endif

