#include "map.hpp"

// todo clean this up
// this is just used for tp imports
struct tp_toggle_pos
{
    std::uint32_t x;
    std::uint32_t y;
    tp_toggle_pos(const std::uint32_t x, const std::uint32_t y): x(x), y(y){}

};

struct tp_toggle_pos_cmp
{
    bool operator()(const tp_toggle_pos& lhs, const tp_toggle_pos&rhs) const
    {
        // todo this will fail for maps over 100000 in size
        // hopefully that never happens lol
        return ((lhs.y*100000) + lhs.x) < ((rhs.y * 100000) + rhs.x);
    }
};

// we have to do this first so we can store the positions
// so we can link this using our toggle_tags system
// second param is id of toggle in toggles
std::map<tp_toggle_pos, std::size_t, tp_toggle_pos_cmp> tp_import_toggle_positions;

int map::tp_import(
    const std::string & json_src,
    const std::string & png_src
) {
    if(tp_import_json(json_src) != 0) {
        std::cerr << "failed loading json" << std::endl;
        return 1;
    }

    if(tp_import_png(png_src) != 0) {
        std::cerr << "failed loading png" << std::endl;
        return 1;
    }

    is_loaded = true;

    return 0;
}

void map::tp_toggle_ref(
    const std::uint32_t x,
    const std::uint32_t y,
    const std::size_t id,
    const toggle_tag_type type
) {
    // see if this is referenced by a toggle
    const tp_toggle_pos toggle_check(x, y);
    if(tp_import_toggle_positions.find(toggle_check)
    != tp_import_toggle_positions.end()) {
        auto & tags = toggles[tp_import_toggle_positions[toggle_check]].tags;
        tags.emplace_back(toggle_tag(id, type));
        std::cout << "added toggle: " << to_string(type) << " x: " << x << " y: " << y << std::endl;
    }
}

int map::tp_import_json(const std::string & src)
{
    using json = nlohmann::json;
    const settings& config = settings::get_instance();

    std::ifstream inputfile_stream(src);
    if(! inputfile_stream.is_open()) {
        std::cerr << "error: unable to load " << src << std::endl;
        return -1;
    }

    json j;
    inputfile_stream >> j;

    std::map<std::string, map_type> maptype_map = {
        {"normal",          map_type::normal},
        {"none-gravityCTF", map_type::none_gravityCTF}
    };
    const auto j_info = j.at("info");
    std::string j_maptype;
    if(j_info.find("gameMode") == j_info.end()) {
        j_maptype = "normal";
    } else {
        j_maptype = j_info.at("gameMode");
    }

    if(maptype_map.find(j_maptype) == maptype_map.end()) {
        std::cerr << "error: map_type \"" << j_maptype << "\" not found" << std::endl;
        return 1;
    }

    this->type    = maptype_map[j_maptype];
    this->name    = j_info.at("name");
    this->author  = j_info.at("author");
    this->version = 1;

    if(j.find("switches") != j.end()) {
        auto j_toggle = j.at("switches");
        for (json::iterator it = j_toggle.begin(); it != j_toggle.end(); ++it) {
            std::vector<std::string> pieces = split_on(it.key(), ',');
            const std::uint32_t x = std::stoi(pieces.at(0));
            const std::uint32_t y = std::stoi(pieces.at(1));

            
            const std::uint32_t timer = (it.value().find("timer") != it.value().end())
                ? it.value()["timer"].get<int>()
                : 0;

            this->toggles.emplace_back(toggle(x, y, timer, {}));

            for(auto p : it.value().at("toggle")) {
                const auto j_pos = p.at("pos");

                tp_import_toggle_positions[
                    tp_toggle_pos(j_pos.at("x"), j_pos.at("y"))
                ] = this->toggles.size() - 1;
            }

            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, config.COLOR_TILE, tile_type::normal));
            }
        }
    }


    if(j.find("portals") != j.end()) {
        auto j_portals = j.at("portals");
        for (json::iterator it = j_portals.begin(); it != j_portals.end(); ++it) {
            std::vector<std::string> pieces = split_on(it.key(), ',');
            const std::uint32_t x = std::stoi(pieces.at(0));
            const std::uint32_t y = std::stoi(pieces.at(1));

            portal p(x, y);

            if(it.value().find("cooldown") != it.value().end()) {
                p.set_cooldown(it.value().at("cooldown"));
            }

            if(it.value().find("destination") != it.value().end()) {
                p.set_destination(
                    it.value().at("destination").at("x"),
                    it.value().at("destination").at("y")
                );
            }

            this->portals.emplace_back(p);
        }
    }

    if(j.find("fields") != j.end()) {
        auto j_fields = j.at("fields");
        for (json::iterator it = j_fields.begin(); it != j_fields.end(); ++it) {
            std::vector<std::string> pieces = split_on(it.key(), ',');
            const std::uint32_t x = std::stoi(pieces.at(0));
            const std::uint32_t y = std::stoi(pieces.at(1));

            std::map<std::string, gate_type> gatetype_map = {
                {"off",  gate_type::off},
                {"on",   gate_type::on},
                {"red",  gate_type::red},
                {"blue", gate_type::blue}
            };
            const std::string j_gatetype = it.value().at("defaultState");

            if(gatetype_map.find(j_gatetype) == gatetype_map.end()) {
                std::cerr << "error: gate_type \"" << j_gatetype << "\" not found" << std::endl;
                return 1;
            }

            gate_type type = gatetype_map[j_gatetype];

            for(auto p : make_square_poly(x, y)) {
                this->gates.emplace_back(gate(p, type));
                tp_toggle_ref(x, y, this->gates.size()-1, toggle_tag_type::gate);
            }
        }
    }

    if(j.find("spawnPoints") != j.end()) {
        auto load_spawnpoints = [&](const spawn_type type) {
            const auto j_spawnpoints = j.at("spawnPoints");
            auto j_spawns = (type == spawn_type::blue)
                ? j_spawnpoints.at("blue")
                : j_spawnpoints.at("red");

            for (json::iterator it = j_spawns.begin(); it != j_spawns.end(); ++it) {
                const std::uint32_t x      = it.value()["x"];
                const std::uint32_t y      = it.value()["y"];
                const std::uint32_t radius = it.value()["radius"];
                const std::uint32_t weight = it.value()["weight"];

                this->spawns.emplace_back(spawn(x, y, radius, weight, type));
            }
        };

        load_spawnpoints(spawn_type::red);
        load_spawnpoints(spawn_type::blue);
    }

    return 0;
}

int map::tp_import_png(const std::string & src)
{
    const settings& config = settings::get_instance();

    std::vector<std::uint8_t> pixels; 
    unsigned error, w, h;

    if((error = lodepng::decode(pixels, w, h, src))) {
        std::cerr
            << "decoder error " << error
            << ": " << lodepng_error_text(error)
            << std::endl;

        return 1;
    }

    this->width  = w;
    this->height = h;

    std::map<std::string, tp_tile_type> tp_tiletype_map  = {
        {"000000", tp_tile_type::background},
        {"d4d4d4", tp_tile_type::tile},
        {"dcbaba", tp_tile_type::speed_red},
        {"bbb8dd", tp_tile_type::speed_blue},
        {"b90000", tp_tile_type::endzone_red},
        {"190094", tp_tile_type::endzone_blue},
        {"787878", tp_tile_type::wall},
        {"408050", tp_tile_type::wall_tl},
        {"405080", tp_tile_type::wall_tr},
        {"807040", tp_tile_type::wall_bl},
        {"804070", tp_tile_type::wall_br},
        {"ff8000", tp_tile_type::bomb},
        {"373737", tp_tile_type::spike},
        {"00ff00", tp_tile_type::powerup},
        {"b97a57", tp_tile_type::button},
        {"ffff00", tp_tile_type::boost_all},
        {"ff7373", tp_tile_type::boost_red},
        {"7373ff", tp_tile_type::boost_blue},
        {"007500", tp_tile_type::gate},
        {"cac000", tp_tile_type::portal},
        {"808000", tp_tile_type::flag_neutral},
        {"ff0000", tp_tile_type::flag_red},
        {"0000ff", tp_tile_type::flag_blue}

    };

    for(std::size_t i=0; i<pixels.size() / 4; ++i) {
        std::uint32_t x = i % w;
        std::uint32_t y = i / w;

        // 4th value is alpha which we dont care about
        std::stringstream ss;
        ss
            << std::hex
            << std::setw(6)
            << std::setfill('0')
            << (
                ((pixels[(i*4) + 0] & 0xff) << 16) | 
                ((pixels[(i*4) + 1] & 0xff) << 8)  | 
                ((pixels[(i*4) + 2] & 0xff) << 0));


        const std::string tile_color = ss.str();

        if(tp_tiletype_map.find(tile_color) == tp_tiletype_map.end()) {
            std::cerr
                << "error: tile_color:  \"" << tile_color << "\" not found"
                << std::endl;
            return 1;
        }

        const tp_tile_type tiletype = tp_tiletype_map[tile_color];

        const color col(tile_color); // we use this for walls, tiles for now for ease
        if(tiletype == tp_tile_type::background) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, col, tile_type::background));
            }
        } else if(tiletype == tp_tile_type::tile) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, col, tile_type::normal));
            }
        } else if(tiletype == tp_tile_type::speed_red) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, col, tile_type::speed_red));
            }
        } else if(tiletype == tp_tile_type::speed_blue) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, col, tile_type::speed_blue));
            }
        } else if(tiletype == tp_tile_type::endzone_red) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, col, tile_type::endzone_red));
            }
        } else if(tiletype == tp_tile_type::endzone_blue) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, col, tile_type::endzone_blue));
            }
        } else if(tiletype == tp_tile_type::wall) {
            for(auto p : make_square_poly(x, y)) {
                walls.emplace_back(wall(p, config.COLOR_WALL));
            }
        } else if(tiletype == tp_tile_type::wall_tl) {
            walls.emplace_back(wall(polygon(x, y, x+1, y, x, y+1), config.COLOR_WALL));
            tiles.emplace_back(tile(polygon(x+1, y, x+1, y+1, x, y+1), config.COLOR_TILE, tile_type::normal));
        } else if(tiletype == tp_tile_type::wall_tr) {
            walls.emplace_back(wall(polygon(x, y, x+1, y, x+1, y+1), config.COLOR_WALL));
            tiles.emplace_back(tile(polygon(x, y, x+1, y+1, x, y+1), config.COLOR_TILE, tile_type::normal));
        } else if(tiletype == tp_tile_type::wall_bl) {
            walls.emplace_back(wall(polygon(x, y, x+1, y+1, x, y+1), config.COLOR_WALL));
            tiles.emplace_back(tile(polygon(x, y, x+1, y, x+1, y+1), config.COLOR_TILE, tile_type::normal));
        } else if(tiletype == tp_tile_type::wall_br) {
            walls.emplace_back(wall(polygon(x+1, y, x+1, y+1, x, y+1), config.COLOR_WALL));
            tiles.emplace_back(tile(polygon(x, y, x+1, y, x, y+1), config.COLOR_TILE, tile_type::normal));
        } else if(tiletype == tp_tile_type::bomb) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, config.COLOR_TILE, tile_type::normal));
            }
            bombs.emplace_back(bomb(x, y));
            tp_toggle_ref(x, y, this->bombs.size()-1, toggle_tag_type::bomb);
        } else if(tiletype == tp_tile_type::spike) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, config.COLOR_TILE, tile_type::normal));
            }
            spikes.emplace_back(spike(x, y));
        } else if(tiletype == tp_tile_type::powerup) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, config.COLOR_TILE, tile_type::normal));
            }
            powerups.emplace_back(powerup(x, y));
        } else if(tiletype == tp_tile_type::button) {
            // this is handled by toggle
        } else if(tiletype == tp_tile_type::boost_all) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, config.COLOR_TILE, tile_type::normal));
            }
            boosts.emplace_back(boost(x, y, boost_type::all));
        } else if(tiletype == tp_tile_type::boost_red) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, config.COLOR_TILE, tile_type::normal));
            }
            boosts.emplace_back(boost(x, y, boost_type::red));
        } else if(tiletype == tp_tile_type::boost_blue) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, config.COLOR_TILE, tile_type::normal));
            }
            boosts.emplace_back(boost(x, y, boost_type::blue));
        } else if(tiletype == tp_tile_type::gate) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, config.COLOR_TILE, tile_type::normal));
            }
            // todo -- this has something to do with json crap
        } else if(tiletype == tp_tile_type::portal) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, config.COLOR_TILE, tile_type::normal));
            }
            // todo -- this has something to do with json crap
        } else if(tiletype == tp_tile_type::flag_neutral) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, col, tile_type::normal));
            }
            flags.emplace_back(flag(x, y, flag_type::neutral));
        } else if(tiletype == tp_tile_type::flag_red) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, col, tile_type::normal));
            }
            flags.emplace_back(flag(x, y, flag_type::red));
        } else if(tiletype == tp_tile_type::flag_blue) {
            for(auto p : make_square_poly(x, y)) {
                tiles.emplace_back(tile(p, col, tile_type::normal));
            }
            flags.emplace_back(flag(x, y, flag_type::blue));
        } else {
            std::cerr << "error: unhandled tile_type: " << tile_color << std::endl;
            return 1;
        }
    }

    return 0;
}

b2World * map::init_world()
{
    b2World* world = new b2World(b2Vec2(0, 0));
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

        for(auto m : boosts) {
            sf::CircleShape s;
            s.setPointCount(3);
            s.setRadius(scaler / 3);
            s.setOrigin(s.getRadius(), s.getRadius());
            s.setPosition((m.x + 0.17) * scaler, (m.y + 0.17)  * scaler);
            switch(m.type) {
                case boost_type::all:  s.setFillColor(sf::Color(250, 250, 70)); break;
                case boost_type::red:  s.setFillColor(sf::Color(255, 75, 25));  break;
                case boost_type::blue: s.setFillColor(sf::Color(25, 75, 255));  break;
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
        {"boosts",   p.boosts},
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
    p.boosts   = j.at("boosts").get<std::vector<boost>>();
    p.gates    = j.at("gates").get<std::vector<gate>>();
    p.flags    = j.at("flags").get<std::vector<flag>>();

    p.is_loaded = true;
}
