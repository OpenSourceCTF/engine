#ifndef ML_MAP_RENDERER_HPP
#define ML_MAP_RENDERER_HPP

#include <iostream>
#include <limits>

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
    sf::Texture tx_sheet;
#endif
    float scaler;
    bool wireframe;

    map_renderer(map& m);

    void display_help() const;
    int get_input();
    int open_window();
    int close_window();
    int render() const;

    template <typename Shape>
    void color_mode(
        Shape& s,
        const color col=color(255, 255, 255, 255)
    ) const {
        const sf::Color draw_color(col.r, col.g, col.b, col.a); 

        if(wireframe) {
            s.setFillColor(sf::Color(0, 0, 0, 0));
            s.setOutlineThickness(-1);
            s.setOutlineColor(draw_color);
        } else {
            s.setFillColor(draw_color);
            s.setOutlineThickness(0);
        }
    }

    template <typename T>
    void std_obj_render(
        const T& o,
        const sf::IntRect txr,
        const bool rotate=false,
        const b2Vec2 offset=b2Vec2(0.0, 0.0),
        const float scale=1.0f
    ) const {
        const b2Vec2 pos = o->body->GetPosition();
        sf::CircleShape s;
        s.setRadius(scaler/2*scale);
        s.setOrigin(s.getRadius(), s.getRadius());
        if(rotate) s.rotate(o->body->GetAngle() * 180 / PI);
        s.setPosition((pos.x + offset.x) * scaler, (pos.y + offset.y)  * scaler);
        color_mode(s);

        if(! wireframe) {
            s.setTexture(&tx_sheet);
            s.setTextureRect(txr);
        }

        window->draw(s);
    }
};

#endif

