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
    sf::RenderWindow* window;
    sf::View view;
    float scaler;
    bool wireframe;

    map_renderer(map& m)
    : m(m)
    , scaler(13.0f)
    , wireframe(false)
    {}

    void display_help();
    int get_input();
    int open_window();
    int close_window();
    int render();
};

#endif

