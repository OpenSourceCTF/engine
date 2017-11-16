#include "map.hpp"

map::map()
: is_loaded(false)
{}


void to_json(nlohmann::json& j, const map& p)
{
    if(! p.is_loaded) {
        spdlog::get("game")->error("map not loaded");
        j = nlohmann::json{{"error", "map not loaded"}};
        return;
    }

    // todo clean me up
    std::vector<wall>    walls;    for(auto && o : p.walls)    walls.emplace_back(*o);
    std::vector<tile>    tiles;    for(auto && o : p.tiles)    tiles.emplace_back(*o);
    std::vector<portal>  portals;  for(auto && o : p.portals)  portals.emplace_back(*o);
    std::vector<toggle>  toggles;  for(auto && o : p.toggles)  toggles.emplace_back(*o);
    std::vector<spawn>   spawns;   for(auto && o : p.spawns)   spawns.emplace_back(*o);
    std::vector<bomb>    bombs;    for(auto && o : p.bombs)    bombs.emplace_back(*o);
    std::vector<spike>   spikes;   for(auto && o : p.spikes)   spikes.emplace_back(*o);
    std::vector<powerup> powerups; for(auto && o : p.powerups) powerups.emplace_back(*o);
    std::vector<booster> boosters; for(auto && o : p.boosters) boosters.emplace_back(*o);
    std::vector<gate>    gates;    for(auto && o : p.gates)    gates.emplace_back(*o);
    std::vector<flag>    flags;    for(auto && o : p.flags)    flags.emplace_back(*o);
    std::vector<chain>   chains;   for(auto && o : p.chains)   chains.emplace_back(*o);

    j = nlohmann::json{
        {"meta", {
            {"type",    to_string(p.type)},
            {"name",    p.name},
            {"author",  p.author},
            {"version", p.version},
            {"width",   p.width},
            {"height",  p.height}
        }},
        {"walls",    walls},
        {"tiles",    tiles},
        {"portals",  portals},
        {"toggles",  toggles},
        {"spawns",   spawns},
        {"bombs",    bombs},
        {"spikes",   spikes},
        {"powerups", powerups},
        {"boosters", boosters},
        {"gates",    gates},
        {"flags",    flags},
        {"chains",   chains}
    };
}

template <typename T> 
std::vector<std::unique_ptr<T>> from_json_helper(
    const nlohmann::json& j,
    const std::string term
) {
    return vec_to_smart_ptr_vec<std::unique_ptr<T>>(j.at(term).get<std::vector<T>>());
}

void from_json(const nlohmann::json& j, map& p)
{
    if(p.is_loaded) {
        spdlog::get("game")->error("map already loaded");
        return;
    }


    auto meta = j.at("meta");
    p.type    = map_type_from_string(meta.at("type").get<std::string>());
    p.name    = meta.at("name").get<std::string>();
    p.author  = meta.at("author").get<std::string>();
    p.version = meta.at("version").get<int>();
    p.width   = meta.at("width").get<int>();
    p.height  = meta.at("height").get<int>();

    p.walls    = from_json_helper<wall>(j, "walls");
    p.tiles    = from_json_helper<tile>(j, "tiles");
    p.portals  = from_json_helper<portal>(j, "portals");

    p.spawns   = from_json_helper<spawn>(j, "spawns");
    p.bombs    = from_json_helper<bomb>(j, "bombs");
    p.spikes   = from_json_helper<spike>(j, "spikes");
    p.toggles  = from_json_helper<toggle>(j, "toggles");
    p.powerups = from_json_helper<powerup>(j, "powerups");
    p.boosters = from_json_helper<booster>(j, "boosters");
    p.gates    = from_json_helper<gate>(j, "gates");
    p.flags    = from_json_helper<flag>(j, "flags");
    p.chains   = from_json_helper<chain>(j, "chains");

    for(auto && o : p.portals) {
        o->destination_ptr = p.portals[o->destination_id].get();
    }

    for(auto && o : p.toggles) {
        for(auto & m : o->tags) {
            switch(m.type) {
                case toggle_tag_type::bomb: m.ptr = p.bombs[m.id].get(); break;
                case toggle_tag_type::gate: m.ptr = p.gates[m.id].get(); break;
            }
        }
    }

    p.is_loaded = true;
}
