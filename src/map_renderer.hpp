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

#ifndef DISABLE_RENDER
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

    template <typename Container>
    sf::VertexArray generate_vertex_array(
        const Container & objs
    ) const {
        const settings& config = settings::get_instance();
        const float e = config.GUI_POLY_EXTRUDE;

        sf::VertexArray polys(sf::Triangles, 3*objs.size());

        for(std::size_t i=0; i<objs.size(); ++i) {
            const polygon poly = objs[i]->poly;
            const b2Vec2 c = poly.get_center();

            const float a1 = std::atan2(poly.v1.y - c.y, poly.v1.x - c.x);
            const float a2 = std::atan2(poly.v2.y - c.y, poly.v2.x - c.x);
            const float a3 = std::atan2(poly.v3.y - c.y, poly.v3.x - c.x);

            polys[(i*3)+0].position = sf::Vector2f(
                (poly.v1.x * scaler) + (std::cos(a1) * e),
                (poly.v1.y * scaler) + (std::sin(a1) * e)
            );
            polys[(i*3)+1].position = sf::Vector2f(
                (poly.v2.x * scaler) + (std::cos(a2) * e),
                (poly.v2.y * scaler) + (std::sin(a2) * e)
            );
            polys[(i*3)+2].position = sf::Vector2f(
                (poly.v3.x * scaler) + (std::cos(a3) * e),
                (poly.v3.y * scaler) + (std::sin(a3) * e)
            );

            polys[(i*3)+0].color = poly.c1;
            polys[(i*3)+1].color = poly.c2;
            polys[(i*3)+2].color = poly.c3;

            polys[(i*3)+0].texCoords = sf::Vector2f(poly.v1.x * 40., poly.v1.y * 40.);
            polys[(i*3)+1].texCoords = sf::Vector2f(poly.v2.x * 40., poly.v2.y * 40.);
            polys[(i*3)+2].texCoords = sf::Vector2f(poly.v3.x * 40., poly.v3.y * 40.);
        }

        return polys;
    }
#endif

};

#endif

