#include "map_renderer.hpp"

map_renderer::map_renderer(map& m)
: m(m)
, scaler(13.0f)
, wireframe(false)
{
#ifndef DISABLE_RENDER
    const settings& config = settings::get_instance();

    const std::string tx_src = "textures/" + config.GUI_TEXTURE + "/tiles.png";
    if(! tx_sheet.loadFromFile(tx_src)) {
        spdlog::get("game")->error("map renderer: couldn't load textures");
    }
#endif
}

void map_renderer::display_help() const
{
    std::cout
        << "gui instructions: " << std::endl
        << std::endl
        << "movement:          arrow keys" << std::endl
        << "select ball:       j/k" << std::endl
        << "wireframe toggle:  p" << std::endl
        << "zoom:              mousewheel" << std::endl;
}

int map_renderer::close_window()
{
#ifdef DISABLE_RENDER
    std::cerr
        << "error: built with DISABLE_RENDER"
        << std::endl;
#else
    if(window && window->isOpen()) {
        window->close();
    }
#endif
    return 0;
}

int map_renderer::open_window()
{
#ifdef DISABLE_RENDER
    std::cerr
        << "error: built with DISABLE_RENDER"
        << std::endl;
    return 1;
#else
    const settings& config = settings::get_instance();

    spdlog::get("game")->debug("map_renderer: attempting to create window");
    sf::ContextSettings settings;
    settings.antialiasingLevel = config.GUI_ANTIALIASING;
    window = new sf::RenderWindow(sf::VideoMode(
        config.GUI_INITIAL_WINDOW_WIDTH,
        config.GUI_INITIAL_WINDOW_HEIGHT
    ), "tagos", sf::Style::Default, settings);
    window->setFramerateLimit(60);

    spdlog::get("game")->debug("map_renderer: window success");

    spdlog::get("game")->debug("map_renderer: attempting to create view");
    view = sf::View(sf::FloatRect(
        0,
        0,
        config.GUI_INITIAL_WINDOW_WIDTH,
        config.GUI_INITIAL_WINDOW_HEIGHT
    ));
    spdlog::get("game")->debug("map_renderer: view success");

    return 0;
#endif
}

int map_renderer::get_input()
{
#ifdef DISABLE_RENDER
    std::cerr
        << "error: built with DISABLE_RENDER"
        << std::endl;
    return 0;
#else
    const settings& config = settings::get_instance();
    static std::size_t current_player = 0;
    sf::Event event;

    const game* g = m.game;

    while(window->pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            window->close();
            return 0;
        }

        if(event.type == sf::Event::Resized) {
            view.setSize(event.size.width, event.size.height);
        }

        if(event.type == sf::Event::MouseWheelMoved) {
            if(event.mouseWheel.delta < 0) {
                scaler *= 0.9;
            } else {
                scaler *= 1.11;
            }
        }


        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Key::P) {
                wireframe = !wireframe;
            }
            if(event.key.code == sf::Keyboard::Key::J) {
                ++current_player;
                current_player %= g->players.size();
            }
            if(event.key.code == sf::Keyboard::Key::K) {
                current_player = current_player > 0
                    ? current_player - 1
                    : g->players.size() - 1;
            }
        }
    }

    if(! g->players.empty()) {
        if(current_player >= g->players.size()) {
            current_player = 0;
        }

        int move_x = 0, move_y = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) move_y--;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move_x--;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) move_y++;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move_x++;
        if(move_x || move_y) {
            player* p = g->players[current_player].get();
            p->xdir = move_x;
            p->ydir = move_y;
        }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        view.move(-scaler/config.GUI_MOVEMENT_SPEED_DIV, 0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        view.move(scaler/config.GUI_MOVEMENT_SPEED_DIV, 0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        view.move(0, -scaler/config.GUI_MOVEMENT_SPEED_DIV);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        view.move(0, scaler/config.GUI_MOVEMENT_SPEED_DIV);
    }

    return 1;
#endif
}

int map_renderer::render() const
{
#ifdef DISABLE_RENDER
    std::cerr
        << "error: built with DISABLE_RENDER"
        << std::endl;
    return 0;
#else
    window->clear(sf::Color::Black);
    window->setView(view);

    auto add_points = [&](
        sf::ConvexShape & s,
        const polygon poly
    ) {
        const settings& config = settings::get_instance();
        const float e = config.GUI_POLY_EXTRUDE;
        const b2Vec2 c = poly.get_center();
        const float a1 = std::atan2(poly.y1 - c.y, poly.x1 - c.x);
        const float a2 = std::atan2(poly.y2 - c.y, poly.x2 - c.x);
        const float a3 = std::atan2(poly.y3 - c.y, poly.x3 - c.x);

        s.setPointCount(3);
        s.setPoint(0, sf::Vector2f(
            (poly.x1 * scaler) + (std::cos(a1) * e),
            (poly.y1 * scaler) + (std::sin(a1) * e)
        ));
        s.setPoint(1, sf::Vector2f(
            (poly.x2 * scaler) + (std::cos(a2) * e),
            (poly.y2 * scaler) + (std::sin(a2) * e)
        ));
        s.setPoint(2, sf::Vector2f(
            (poly.x3 * scaler) + (std::cos(a3) * e),
            (poly.y3 * scaler) + (std::sin(a3) * e)
        ));
    };

    auto make_thick_line = [&](const coord& a, const coord& b) -> sf::RectangleShape
    {
        const sf::Vector2f pos(scaler * a.x, scaler * a.y);
        const float w = std::pow(
            std::pow(scaler * (a.x - b.x), 2) + std::pow(scaler * (a.y - b.y), 2),
            0.5
        );
        const float angle = std::atan2(b.y - a.y, b.x - a.x);

        sf::RectangleShape res(sf::Vector2f(w, 3));
        res.setPosition(pos);
        res.setRotation(angle * 180 / PI);
        return res;
    };

    if(! wireframe) {
        for(auto && o : m.walls) {
            sf::ConvexShape s;
            add_points(s, o->poly);
            s.setTexture(&tx_sheet);
            s.setTextureRect(sf::IntRect(80, 120, 40, 40));

            window->draw(s);
        }
    } else {
        std::size_t i = 0;

        for(const auto& ch : m.chains) {
            sf::VertexArray lines(sf::Lines, ch->vertices.size());

            for(auto it = ch->vertices.begin(); it != ch->vertices.end(); ++it) {
                if(std::next(it) != ch->vertices.end()) {
                    sf::RectangleShape rect(make_thick_line(*it, *std::next(it)));

                    rect.setFillColor(sf::Color(
                        DISTINCT_COLORS[i%63][0],
                        DISTINCT_COLORS[i%63][1],
                        DISTINCT_COLORS[i%63][2]
                    ));

                    window->draw(rect);
                }
            }

            ++i;
        }
    }
    
    for(auto && o : m.tiles) {
        sf::ConvexShape s;
        add_points(s, o->poly);
        color_mode(s, o->col);

        if(! wireframe) {
            s.setTexture(&tx_sheet);
            s.setTextureRect(sf::IntRect(120, 120, 40, 40));
        }

        window->draw(s);
    }

    for(auto && o : m.gates) {
        sf::ConvexShape s;
        add_points(s, o->poly);
        color_mode(s);

        const sf::IntRect off(120, 80, 40, 40);
        const sf::IntRect green(160, 80, 40, 40);
        const sf::IntRect red(0, 120, 40, 40);
        const sf::IntRect blue(40, 120, 40, 40);

        if(! wireframe) {
            s.setTexture(&tx_sheet);

            switch(o->current) {
                case gate_type::off:   s.setTextureRect(off);   break;
                case gate_type::green: s.setTextureRect(green); break;
                case gate_type::red:   s.setTextureRect(red);   break;
                case gate_type::blue:  s.setTextureRect(blue);  break;
            }
        }

        window->draw(s);
    }

    for(auto && o : m.portals) {
        std_obj_render(o, sf::IntRect(0, 160, 40, 40));
    }

    for(auto && o : m.bombs) {
        if(! o->is_alive) continue;
        std_obj_render(o, sf::IntRect(160, 160, 40, 40));
    }

    for(auto && o : m.spikes) {
        std_obj_render(o, sf::IntRect(40, 160, 40, 40));
    }
    
    for(auto && o : m.powerups) {
        if(! o->is_alive) continue;
        const sf::IntRect tagpro(0, 80, 40, 40);
        const sf::IntRect jukejuice(120, 40, 40, 40);
        const sf::IntRect rollingbomb(160, 40, 40, 40);

        switch(o->type) {
            case powerup_type::tagpro:      std_obj_render(o, tagpro);      break;
            case powerup_type::jukejuice:   std_obj_render(o, jukejuice);   break;
            case powerup_type::rollingbomb: std_obj_render(o, rollingbomb); break;
        } 
    }
    
    for(auto && o : m.toggles) {
        std_obj_render(o, sf::IntRect(80, 80, 40, 40));
    }

    for(auto && o : m.flags) {
        // normal:taken
        const std::pair<sf::IntRect, sf::IntRect> blue = {
            sf::IntRect(40, 40, 40, 40),
            sf::IntRect(80, 40, 40, 40)
        };
        const std::pair<sf::IntRect, sf::IntRect> red = {
            sf::IntRect(160, 0, 40, 40),
            sf::IntRect(0, 40, 40, 40)
        };
        const std::pair<sf::IntRect, sf::IntRect> yellow = {
            sf::IntRect(80, 0, 40, 40),
            sf::IntRect(120, 0, 40, 40)
        };

        const bool taken = !o->is_alive;

        switch(o->type) {
            case flag_type::neutral: std_obj_render(o, taken ? yellow.second : yellow.first); break;
            case flag_type::blue:    std_obj_render(o, taken ? blue.second : blue.first);     break;
            case flag_type::red:     std_obj_render(o, taken ? red.second : red.first);       break;
        }
    }

    for(auto && o : m.boosters) {
        if(! o->is_alive) continue;

        const sf::IntRect blue(40, 80, 40, 40);
        const sf::IntRect red(160, 120, 40, 40);
        const sf::IntRect yellow(0, 0, 40, 40);

        switch(o->type) {
            case booster_type::all:  std_obj_render(o, yellow); break;
            case booster_type::red:  std_obj_render(o, red);    break;
            case booster_type::blue: std_obj_render(o, blue);   break;
        }
    }

    for(auto && o : m.balls) {
        if(! o->is_alive) continue;

        const sf::IntRect red(80, 160, 40, 40);
        const sf::IntRect blue(120, 80, 40, 40);

        switch(o->type) {
            case ball_type::red:  std_obj_render(o, red, true);  break;
            case ball_type::blue: std_obj_render(o, blue, true); break;
        }

        /*
        if(! o->powerups.empty()) {
            s.setOutlineThickness(2);
            s.setOutlineColor(sf::Color(
                (o->has_powerup(powerup_type::jukejuice)   ? 255 : 0),
                (o->has_powerup(powerup_type::tagpro)      ? 255 : 0),
                (o->has_powerup(powerup_type::rollingbomb) ? 255 : 0),
                255
            ));
        }*/

        if(! o->flags.empty()) {
            const sf::IntRect blue(40, 40, 40, 40);
            const sf::IntRect red(160, 0, 40, 40);
            const sf::IntRect yellow(80, 0, 40, 40);

            switch(o->flags.back().f->type) {
                case flag_type::neutral: std_obj_render(o, yellow); break;
                case flag_type::blue:    std_obj_render(o, blue);   break;
                case flag_type::red:     std_obj_render(o, red);    break;
            }
        }
    }

    window->display();
    return 1;
#endif  
}

