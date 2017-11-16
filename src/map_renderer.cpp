#include "map_renderer.hpp"

map_renderer::map_renderer(map& m)
: m(m)
, scaler(13.0f)
, wireframe(false)
{}

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
    return 0;
#else
    if(window && window->isOpen()) {
        window->close();
    }
    return 0;
#endif
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
    window = new sf::RenderWindow(sf::VideoMode(
        config.GUI_INITIAL_WINDOW_WIDTH,
        config.GUI_INITIAL_WINDOW_HEIGHT
    ), "tagos");
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
    static int current_ball = 0; // which to move
    sf::Event event;

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
                ++current_ball;
                current_ball %= m.balls.size();
            }
            if(event.key.code == sf::Keyboard::Key::K) {
                --current_ball;
                current_ball %= m.balls.size();
            }
        }
    }

    if(! m.balls.empty()) {
        // CONTROL
        int move_x = 0, move_y = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) move_y--;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move_x--;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) move_y++;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move_x++;
        if(move_x || move_y) m.balls[current_ball]->move(move_x, move_y);
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

    auto color_mode = [&](
        sf::ConvexShape & s,
        const color col
    ) {
        const sf::Color draw_color(col.r, col.g, col.b); 

        if(wireframe) {
            s.setFillColor(sf::Color(0, 0, 0, 0));
            s.setOutlineThickness(-1);
            s.setOutlineColor(draw_color);
        } else {
            s.setFillColor(draw_color);
            s.setOutlineThickness(0);
        }
    };

    auto add_points = [&](
        sf::ConvexShape & s,
        const polygon poly
    ) {
        s.setPointCount(3);
        s.setPoint(0, sf::Vector2f(poly.x1 * scaler, poly.y1 * scaler));
        s.setPoint(1, sf::Vector2f(poly.x2 * scaler, poly.y2 * scaler));
        s.setPoint(2, sf::Vector2f(poly.x3 * scaler, poly.y3 * scaler));
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
            color_mode(s, o->col);
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
        window->draw(s);
    }

    for(auto && o : m.gates) {
        sf::ConvexShape s;
        add_points(s, o->poly);
        switch(o->current) {
            case gate_type::off:  s.setFillColor(sf::Color(70,   70,   70)); break;
            case gate_type::green:   s.setFillColor(sf::Color(0,   255, 0)); break;
            case gate_type::blue: s.setFillColor(sf::Color(0,   0,   255)); break;
            case gate_type::red:  s.setFillColor(sf::Color(255, 0,   0)); break;
        }
        window->draw(s);
    }

    for(auto && o : m.portals) {
        sf::CircleShape s;
        s.setRadius(scaler / 2);
        s.setOrigin(s.getRadius(), s.getRadius());
        s.setPosition(o->x * scaler, o->y  * scaler);
        const int alpha = o->is_alive ? 255 : 100;
        s.setFillColor(sf::Color(255, 0, 232, alpha));
        window->draw(s);
    }

    for(auto && o : m.bombs) {
        if(! o->is_alive) continue;
        sf::CircleShape s;
        s.setPointCount(8);
        s.setRadius(scaler / 2);
        s.setOrigin(s.getRadius(), s.getRadius());
        s.setPosition(o->x * scaler, o->y * scaler);
        s.setFillColor(sf::Color(30, 30, 30));
        window->draw(s);
    }

    for(auto && o : m.spikes) {
        sf::CircleShape s;
        s.setPointCount(3);
        s.setRadius(scaler / 2);
        s.setOrigin(s.getRadius(), s.getRadius());
        s.setPosition(o->x * scaler, o->y  * scaler);
        s.setFillColor(sf::Color(70, 70, 70));
        window->draw(s);
    }
    
    for(auto && o : m.powerups) {
        if(! o->is_alive) continue;
        sf::CircleShape s;
        s.setPointCount(5);
        s.setRadius(scaler / 2);
        s.setOrigin(s.getRadius(), s.getRadius());
        s.setPosition(o->x * scaler, o->y  * scaler);
        s.setFillColor(sf::Color(30, 255, 30));

        s.setOutlineThickness(2);
        switch(o->type) {
            case powerup_type::tagpro:      s.setOutlineColor(sf::Color(255, 0, 0, 255)); break;
            case powerup_type::jukejuice:   s.setOutlineColor(sf::Color(0, 255, 0, 255)); break;
            case powerup_type::rollingbomb: s.setOutlineColor(sf::Color(0, 0, 255, 255)); break;
        }

        window->draw(s);
    }
    
    for(auto && o : m.toggles) {
        sf::CircleShape s;
        s.setRadius(scaler / 4);
        s.setPosition((o->x + 0.25) * scaler, (o->y + 0.25)  * scaler);
        s.setOrigin(s.getRadius(), s.getRadius());
        s.setFillColor(sf::Color(230, 200, 100));
        window->draw(s);
    }

    for(auto && o : m.flags) {
        sf::CircleShape s;
        s.setPointCount(3);
        s.setRadius(scaler / 2);
        s.setOrigin(s.getRadius(), s.getRadius());
        s.setPosition(o->x * scaler, o->y * scaler);
        const int alpha = o->is_alive ? 255 : 100;
        switch(o->type) {
            case flag_type::neutral: s.setFillColor(sf::Color(250, 240, 20, alpha)); break;
            case flag_type::blue:    s.setFillColor(sf::Color(30, 30, 170, alpha)); break;
            case flag_type::red:     s.setFillColor(sf::Color(170, 30, 30, alpha)); break;
        }
        window->draw(s);
    }

    for(auto && o : m.boosters) {
        if(! o->is_alive) continue;
        sf::CircleShape s;
        s.setPointCount(3);
        s.setRadius(scaler / 3);
        s.setOrigin(s.getRadius(), s.getRadius());
        s.setPosition((o->x + 0.17) * scaler, (o->y + 0.17)  * scaler);
        switch(o->type) {
            case booster_type::all:  s.setFillColor(sf::Color(250, 250, 70)); break;
            case booster_type::red:  s.setFillColor(sf::Color(255, 75, 25));  break;
            case booster_type::blue: s.setFillColor(sf::Color(25, 75, 255));  break;
        }
        window->draw(s);
    }

    for(auto && o : m.balls) {
        if(! o->is_alive) continue;
        b2Vec2 pos = o->body->GetPosition();
        sf::CircleShape s;
        s.setRadius(scaler / 2);
        s.setOrigin(s.getRadius(), s.getRadius());
        s.setPosition(pos.x * scaler, pos.y  * scaler);
        switch(o->type) {
            case ball_type::red:  s.setFillColor(sf::Color(200, 50, 50)); break;
            case ball_type::blue: s.setFillColor(sf::Color(50, 50, 200)); break;
        }

        if(! o->powerups.empty()) {
            s.setOutlineThickness(2);
            s.setOutlineColor(sf::Color(
                (o->has_powerup(powerup_type::tagpro)      ? 255 : 0),
                (o->has_powerup(powerup_type::jukejuice)   ? 255 : 0),
                (o->has_powerup(powerup_type::rollingbomb) ? 255 : 0),
                255
            ));
        }

        window->draw(s);

        if(! o->flags.empty()) {
            const flag* f = o->flags[0].f;

            sf::CircleShape s;
            s.setRadius(scaler / 4);
            s.setOrigin(s.getRadius(), s.getRadius());
            s.setPosition(pos.x * scaler, pos.y  * scaler);
            switch(f->type) {
                case flag_type::neutral: s.setFillColor(sf::Color(250, 240, 10)); break;
                case flag_type::blue:    s.setFillColor(sf::Color(70, 70, 230));  break;
                case flag_type::red:     s.setFillColor(sf::Color(230, 70, 70));  break;
            }

            window->draw(s);
        }
    }

    window->display();
    return 1;
#endif  
}

