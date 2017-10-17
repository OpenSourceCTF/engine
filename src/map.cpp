#include "map.hpp"

map::map()
: is_loaded(false)
{
    // set toggle map pointer
    toggle::get_map(this);
}

void map::update(b2World* world)
{
    for(auto o : balls) {
        if(! o->is_alive) respawn_ball(o);
        o->is_alive = true;
    }
}

void map::respawn_ball(ball* b)
{
    std::cout << "respawning" << std::endl;
    random_util& rng = random_util::get_instance();

    std::vector<spawn> potential_spawns;

    for(auto s : spawns) {
        if((s.type == spawn_type::red  && b->type == ball_type::red)
        || (s.type == spawn_type::blue && b->type == ball_type::blue)
        ) {
            for(std::size_t i=0; i<(s.weight > 0) ? s.weight : 1; ++i) {
                potential_spawns.emplace_back(s);
            }
        }
    }

    if(potential_spawns.empty()) {
        const spawn_type matching_spawn_type = b->type == ball_type::red
            ? spawn_type::red
            : spawn_type::blue;

        // maps dont require spawn points...
        // but thats stupid
        // maybe this should be fixed in map export
        const flag_type matching_flag_type = b->type == ball_type::red
            ? flag_type::red
            : flag_type::blue;

        for(auto f : flags) {
            if(f.type == matching_flag_type) {
                potential_spawns.emplace_back(spawn(f.x, f.y, 1, 1, matching_spawn_type));
            }
        }

        // if theres no flags or spawns...
        if(potential_spawns.empty()) {
            std::cerr << "error: no spawns found, placing at 0,0, fix your map" << std::endl;
            potential_spawns.emplace_back(spawn(0, 0, 1, 1, matching_spawn_type));
        }
    }

    const spawn & s = potential_spawns[
        std::uniform_int_distribution<int>(0, potential_spawns.size()-1)(rng.eng)
    ];
    const float a = std::uniform_real_distribution<>(0.0f, TWO_PI)(rng.eng);
    b->set_position(b2Vec2(
        s.x + (std::cos(a) * s.radius),
        s.y + (std::sin(a) + s.radius))
    );
}

ball* map::add_ball(b2World* world, ball b)
{
    ball* o = new ball(b);
    o->add_to_world(world);
    balls.emplace_back(o);
    respawn_ball(o);

    return o;
}


b2World * map::init_world()
{
    b2World* world = new b2World(b2Vec2(0, 0));
    static contact_listener contact_listener_instance;
    world->SetContactListener(&contact_listener_instance);

    for(std::size_t i=0; i<8; ++i) {
        add_ball(world, ball(i < 4 ? ball_type::red : ball_type::blue));
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

    for(auto & m : toggles) {
        m.add_to_world(world);
    }

    for(auto & m : boosters) {
        m.add_to_world(world);
    }

    for(auto & m : powerups) {
        m.add_to_world(world);
    }

    for(auto & m : flags) {
        m.add_to_world(world);
    }

    for(auto & m : portals) {
        m.add_to_world(world);
    }

    return world;
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
