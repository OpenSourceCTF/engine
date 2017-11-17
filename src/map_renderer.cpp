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
    const settings& config = settings::get_instance();
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

    std::ostringstream oss;
    oss << "textures/" << config.DEFAULT_TEXTURE << "/tiles.png";
    auto tx_file = oss.str();

    sf::Texture tx_sheet;
    if(!tx_sheet.loadFromFile(tx_file)) {
        spdlog::get("game")->error("map renderer: couldn't load textures");
    }
    const sf::Texture const_tx_sheet = tx_sheet;

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

    auto configure_sprite = [&](sf::Sprite& s, b2Body* body) {
        b2AABB aabb;
        b2Transform t;
        t.SetIdentity();
        aabb.lowerBound = b2Vec2(FLT_MAX,FLT_MAX);
        aabb.upperBound = b2Vec2(-FLT_MAX,-FLT_MAX);
        b2Fixture* fixture = body->GetFixtureList();
        while (fixture != nullptr) {
             const b2Shape *shape = fixture->GetShape();
             const int childCount = shape->GetChildCount();
             for (int child = 0; child < childCount; ++child) {
                    const b2Vec2 r(shape->m_radius, shape->m_radius);
                    b2AABB shapeAABB;
                    shape->ComputeAABB(&shapeAABB, t, child);
                    shapeAABB.lowerBound = shapeAABB.lowerBound + r;
                    shapeAABB.upperBound = shapeAABB.upperBound - r;
                    aabb.Combine(shapeAABB);
            }
            fixture = fixture->GetNext();
        }
        s.setPosition(body->GetPosition().x * scaler, body->GetPosition().y * scaler);
        /*
        s.setOrigin(scaler * (body->GetWorldCenter().x
                              -aabb.lowerBound.x),
                    scaler * (body->GetWorldCenter().y
                              -aabb.lowerBound.y));
                              */
    };

    if(! wireframe) {
        for(auto && o : m.walls) {
            sf::ConvexShape s;
            add_points(s, o->poly);
            s.setTexture(&tx_sheet);
            const sf::IntRect R(80, 120, 40, 40);
            s.setTextureRect(R);
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
        s.setTexture(&tx_sheet);
        const sf::IntRect R(120, 120, 40, 40);
        s.setTextureRect(R);
        window->draw(s);
    }

    for(auto && o : m.gates) {
        sf::Sprite s;
        configure_sprite(s,o->body);
        s.setTexture(const_tx_sheet);
        const sf::IntRect off(120, 80, 40, 40);
        const sf::IntRect green(160, 80, 40, 40);
        const sf::IntRect red(0, 120, 40, 40);
        const sf::IntRect blue(40, 120, 40, 40);
        switch(o->current) {
            case gate_type::off:  s.setTextureRect(off); break;
            case gate_type::green:   s.setTextureRect(green); break;
            case gate_type::blue: s.setTextureRect(blue); break;
            case gate_type::red:  s.setTextureRect(red); break;
        }
        window->draw(s);
    }

    for(auto && o : m.portals) {
        sf::Sprite s;
        configure_sprite(s,o->body);
        s.setTexture(const_tx_sheet);
        const sf::IntRect R(0, 160, 40, 40);
        s.setTextureRect(R);
        window->draw(s);
    }

    for(auto && o : m.bombs) {
        if(! o->is_alive) continue;
        sf::Sprite s;
        configure_sprite(s,o->body);
        s.setTexture(const_tx_sheet);
        const sf::IntRect R(160, 160, 40, 40);
        s.setTextureRect(R);
        window->draw(s);
    }

    for(auto && o : m.spikes) {
        sf::Sprite s;
        configure_sprite(s,o->body);
        s.setTexture(const_tx_sheet);
        const sf::IntRect R(40, 160, 40, 40);
        s.setTextureRect(R);
        window->draw(s);
    }
    
    for(auto && o : m.powerups) {
        if(! o->is_alive) continue;
        sf::Sprite s;
        configure_sprite(s,o->body);
        s.setTexture(const_tx_sheet);
        const sf::IntRect tp(0, 80, 40, 40);
        const sf::IntRect rb(160, 40, 40, 40);
        const sf::IntRect jj(120, 40, 40, 40);

        switch(o->type) {
            case powerup_type::tagpro:      s.setTextureRect(tp); break;
            case powerup_type::jukejuice:   s.setTextureRect(jj); break;
            case powerup_type::rollingbomb: s.setTextureRect(rb); break;
        }

        window->draw(s);
    }
    
    for(auto && o : m.toggles) {
        sf::Sprite s;
        configure_sprite(s,o->body);
        s.setTexture(const_tx_sheet);
        const sf::IntRect R(80, 80, 40, 40);
        s.setTextureRect(R);
        window->draw(s);
    }

    for(auto && o : m.flags) {
        sf::Sprite s;
        configure_sprite(s,o->body);
        s.setTexture(const_tx_sheet);
        const sf::IntRect blue(40, 40, 40, 40);
        const sf::IntRect blue_taken(80, 40, 40, 40);
        const sf::IntRect red(160, 0, 40, 40);
        const sf::IntRect red_taken(0, 40, 40, 40);
        const sf::IntRect yellow(80, 0, 40, 40);
        const sf::IntRect yellow_taken(120, 0, 40, 40);
        switch(o->type) {
            case flag_type::neutral: s.setTextureRect(o->is_alive ? yellow : yellow_taken); break;
            case flag_type::blue:    s.setTextureRect(o->is_alive ? blue : blue_taken); break;
            case flag_type::red:     s.setTextureRect(o->is_alive ? red : red_taken); break;
        }
        window->draw(s);
    }

    for(auto && o : m.boosters) {
        if(! o->is_alive) continue;
        sf::Sprite s;
        configure_sprite(s,o->body);
        s.setTexture(const_tx_sheet);
        const sf::IntRect blue(40, 80, 40, 40);
        const sf::IntRect red(160, 120, 40, 40);
        const sf::IntRect yellow(0, 0, 40, 40);
        switch(o->type) {
            case booster_type::all:  s.setTextureRect(yellow); break;
            case booster_type::red:  s.setTextureRect(red);  break;
            case booster_type::blue: s.setTextureRect(blue);  break;
        }
        window->draw(s);
    }

    for(auto && o : m.balls) {
        if(! o->is_alive) continue;
        sf::Sprite s;
        configure_sprite(s,o->body);
        s.setTexture(const_tx_sheet);
        s.rotate(o->body->GetAngle() * 180 / PI);
        
        const sf::IntRect red(80, 160, 40, 40);
        const sf::IntRect blue(120, 80, 40, 40);
        switch(o->type) {
            case ball_type::red:  s.setTextureRect(red); break;
            case ball_type::blue: s.setTextureRect(blue); break;
        }

        if(! o->powerups.empty()) {
            /*
            s.setOutlineThickness(2);
            s.setOutlineColor(sf::Color(
                (o->has_powerup(powerup_type::jukejuice)   ? 255 : 0),
                (o->has_powerup(powerup_type::tagpro)      ? 255 : 0),
                (o->has_powerup(powerup_type::rollingbomb) ? 255 : 0),
                255
            ));
            */
        }

        window->draw(s);

        if(! o->flags.empty()) {
            const flag* f = o->flags[0].f;

            sf::CircleShape sh;
            sf::Sprite s;
            configure_sprite(s,o->body);
            const sf::IntRect blue(40, 40, 40, 40);
            const sf::IntRect red(160, 0, 40, 40);
            const sf::IntRect yellow(80, 0, 40, 40);
            switch(f->type) {
                case flag_type::neutral: s.setTextureRect(yellow); break;
                case flag_type::blue:    s.setTextureRect(blue); break;
                case flag_type::red:     s.setTextureRect(red); break;
            }

            window->draw(s);
        }
    }

    window->display();
    return 1;
#endif  
}

