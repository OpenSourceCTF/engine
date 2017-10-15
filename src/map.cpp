#include "map.hpp"

b2World * map::init_world()
{
    b2World* world = new b2World(b2Vec2(0, 0));
    static contact_listener contact_listener_instance;
    world->SetContactListener(&contact_listener_instance);

    {
        ball me(ball_type::red);
        me.add_to_world(world);
        balls.emplace_back(me);
    }

    for(auto & m : walls) {
        m.add_to_world(world);
    }

    for(auto & m : spikes) {
        m.add_to_world(world);
    }

    for(auto & m : bombs) {
        m.add_to_world(world);
    }

    return world;
}

int map::render()
{
#ifdef DISABLE_RENDER
    std::cerr
        << "error: built with DISABLE_RENDER"
        << std::endl;
    return 1;
#else
    const settings& config = settings::get_instance();

    sf::RenderWindow window(sf::VideoMode(
        config.GUI_INITIAL_WINDOW_WIDTH,
        config.GUI_INITIAL_WINDOW_HEIGHT
    ), "tagos");

    sf::View view(sf::FloatRect(
        0,
        0,
        config.GUI_INITIAL_WINDOW_WIDTH,
        config.GUI_INITIAL_WINDOW_HEIGHT
    ));

    float scaler   = 13.0f;
    bool wireframe = false;

    std::cout
        << "gui instructions: " << std::endl
        << std::endl
        << "movement: arrow keys" << std::endl
        << "wireframe toggle: p" << std::endl
        << "zoom: mousewheel" << std::endl;

    b2World * world = init_world();

	while(window.isOpen()) {
		sf::Event event;

		while(window.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				window.close();
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
            }
		}

        {
            // CONTROL
            int move_x = 0, move_y = 0;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) move_y--;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move_x--;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) move_y++;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move_x++;
            if(move_x || move_y) balls[0].move(move_x, move_y);
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

		window.clear(sf::Color::Black);
		window.setView(view);

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

        for(auto m : walls) {
            sf::ConvexShape s;
            add_points(s, m.poly);
            color_mode(s, m.col);
    		window.draw(s);
        }
		
        for(auto m : tiles) {
            sf::ConvexShape s;
            add_points(s, m.poly);
            color_mode(s, m.col);
    		window.draw(s);
        }

        for(auto m : gates) {
            sf::ConvexShape s;
            add_points(s, m.poly);
            switch(m.type) {
                case gate_type::off:  s.setFillColor(sf::Color(0,   0,   0,   50)); break;
                case gate_type::on:   s.setFillColor(sf::Color(0,   255, 0,   50)); break;
                case gate_type::blue: s.setFillColor(sf::Color(0,   0,   255, 50)); break;
                case gate_type::red:  s.setFillColor(sf::Color(255, 0,   0,   50)); break;
            }
    		window.draw(s);
        }

        for(auto m : portals) {
            sf::CircleShape s;
            s.setRadius(scaler / 2);
            s.setOrigin(s.getRadius(), s.getRadius());
            s.setPosition(m.x * scaler, m.y  * scaler);
            s.setFillColor(sf::Color(255, 0, 232));
            window.draw(s);
        }

        for(auto m : bombs) {
            sf::CircleShape s;
            s.setPointCount(8);
            s.setRadius(scaler / 3);
            s.setOrigin(s.getRadius(), s.getRadius());
            s.setPosition((m.x + 0.17) * scaler, (m.y + 0.17)  * scaler);
            s.setFillColor(sf::Color(30, 30, 30));
            window.draw(s);
        }

        for(auto m : spikes) {
            sf::CircleShape s;
            s.setPointCount(3);
            s.setRadius(scaler / 2);
            s.setOrigin(s.getRadius(), s.getRadius());
            s.setPosition(m.x * scaler, m.y  * scaler);
            s.setFillColor(sf::Color(70, 70, 70));
            window.draw(s);
        }
		
        for(auto m : powerups) {
            sf::CircleShape s;
            s.setPointCount(5);
            s.setRadius(scaler / 2);
            s.setOrigin(s.getRadius(), s.getRadius());
            s.setPosition(m.x * scaler, m.y  * scaler);
            s.setFillColor(sf::Color(30, 255, 30));
            window.draw(s);
        }
		
        for(auto m : toggles) {
            sf::CircleShape s;
            s.setRadius(scaler / 4);
            s.setPosition((m.x + 0.25) * scaler, (m.y + 0.25)  * scaler);
            s.setOrigin(s.getRadius(), s.getRadius());
            s.setFillColor(sf::Color(230, 200, 100));
            window.draw(s);
        }

        for(auto m : boosters) {
            sf::CircleShape s;
            s.setPointCount(3);
            s.setRadius(scaler / 3);
            s.setOrigin(s.getRadius(), s.getRadius());
            s.setPosition((m.x + 0.17) * scaler, (m.y + 0.17)  * scaler);
            switch(m.type) {
                case booster_type::all:  s.setFillColor(sf::Color(250, 250, 70)); break;
                case booster_type::red:  s.setFillColor(sf::Color(255, 75, 25));  break;
                case booster_type::blue: s.setFillColor(sf::Color(25, 75, 255));  break;
            }
            window.draw(s);
        }

        for(auto m : balls) {
            b2Vec2 pos = m.get_position();
            sf::CircleShape s;
            s.setRadius(scaler / 2);
            s.setOrigin(s.getRadius(), s.getRadius());
            s.setPosition(pos.x * scaler, pos.y  * scaler);
            switch(m.type) {
                case ball_type::red:  s.setFillColor(sf::Color(200, 50, 50)); break;
                case ball_type::blue: s.setFillColor(sf::Color(50, 50, 200)); break;
            }
            window.draw(s);
        }
		
        window.display();
        world->Step(1/60.0, 8, 3);
	}

    return 0;
#endif  
}

void to_json(nlohmann::json& j, const map& p)
{
    if(! p.is_loaded) {
        std::cerr << "error: map not loaded" << std::endl;
        j = nlohmann::json{{"error", "map not loaded"}};
        return;
    }

    j = nlohmann::json{
        {"meta", {
            {"type",    to_string(p.type)},
            {"name",    p.name},
            {"author",  p.author},
            {"version", p.version},
            {"width",   p.width},
            {"height",  p.height}
        }},
        {"walls",   p.walls},
        {"tiles",   p.tiles},
        {"portals", p.portals},
        {"toggles", p.toggles},
        {"spawns",   p.spawns},
        {"bombs",    p.bombs},
        {"spikes",   p.spikes},
        {"powerups", p.powerups},
        {"boosters", p.boosters},
        {"gates",    p.gates},
        {"flags",    p.flags}
    };
}

void from_json(const nlohmann::json& j, map& p)
{
    if(p.is_loaded) {
        std::cerr << "error: map already loaded" << std::endl;
        return;
    }

    auto meta = j.at("meta");
    p.type    = map_type_from_string(meta.at("type").get<std::string>());
    p.name    = meta.at("name").get<std::string>();
    p.author  = meta.at("author").get<std::string>();
    p.version = meta.at("version").get<int>();
    p.width   = meta.at("width").get<int>();
    p.height  = meta.at("height").get<int>();

    p.walls   = j.at("walls").get<std::vector<wall>>();
    p.tiles   = j.at("tiles").get<std::vector<tile>>();
    p.portals = j.at("portals").get<std::vector<portal>>();

    p.spawns   = j.at("spawns").get<std::vector<spawn>>();
    p.bombs    = j.at("bombs").get<std::vector<bomb>>();
    p.spikes   = j.at("spikes").get<std::vector<spike>>();
    p.toggles  = j.at("toggles").get<std::vector<toggle>>();
    p.powerups = j.at("powerups").get<std::vector<powerup>>();
    p.boosters = j.at("boosters").get<std::vector<booster>>();
    p.gates    = j.at("gates").get<std::vector<gate>>();
    p.flags    = j.at("flags").get<std::vector<flag>>();

    p.is_loaded = true;
}
