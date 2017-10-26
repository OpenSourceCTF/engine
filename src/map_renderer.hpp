#ifndef ML_MAP_RENDERER_HPP
#define ML_MAP_RENDERER_HPP

#include <iostream>

#ifndef DISABLE_RENDER
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#endif

#include "map.hpp"
#include "settings.hpp"

struct map_renderer
{
    map& m;
#ifndef DISABLE_RENDER
    sf::RenderWindow* window;
    sf::View view;
#endif
    float scaler;
    bool wireframe;

    map_renderer(map& m)
    : m(m)
    , scaler(13.0f)
    , wireframe(false)
    {}

    void display_help() const;
    int get_input();
    int open_window();
    int close_window();
    int render() const;
};

#endif

