#include "map.hpp"

map::map()
: is_loaded(false)
{
    // set toggle map pointer
    toggle::get_map(this);
}

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

    for(auto & m : toggles) {
        m.add_to_world(world);
    }

    for(auto & m : boosters) {
        m.add_to_world(world);
    }

    for(auto & m : powerups) {
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
