#include "tp_map_importer.hpp"
#include <json/json.hpp>


// the types of tiles from tagpro .png map
enum class tp_tile_type
{
    background,
    tile,
    speed_red,
    speed_blue,
    speed_yellow,
    endzone_red,
    endzone_blue,
    wall,
    wall_tl,
    wall_tr,
    wall_bl,
    wall_br,
    bomb,
    spike,
    powerup,
    button,
    booster_all,
    booster_red,
    booster_blue,
    gate,
    portal,
    flag_neutral,
    flag_red,
    flag_blue,
    respawn_red,
    respawn_blue,
    gravitywell,
    marsball,
};


// todo clean this up
// this is just used for tp imports
struct tp_pos
{
    std::uint32_t x;
    std::uint32_t y;
    tp_pos(const std::uint32_t x, const std::uint32_t y): x(x), y(y){}

    bool operator <(const tp_pos& rhs) const
    {
        // todo this will fail for maps over 100000 in size
        // hopefully that never happens lol
        return ((y*100000) + x) < ((rhs.y * 100000) + rhs.x);
    }
};

struct tp_pos_cmp
{
    bool operator()(const tp_pos& lhs, const tp_pos&rhs) const
    {
        return lhs < rhs;
    }
};

// we have to do this first so we can store the positions
// so we can link this using our toggle_tags system
// second param is id of toggle in toggles
static std::map<tp_pos, std::vector<std::size_t>, tp_pos_cmp> tp_import_toggle_positions;
static std::map<tp_pos, std::size_t, tp_pos_cmp> tp_import_portal_positions;

// little helper for turning tiles into 2 polys
std::vector<polygon> make_square_poly(
    const float x,
    const float y
) {
    return {
        polygon(x - 0.5f, y - 0.5f, x + 0.5f, y - 0.5f, x + 0.5f, y + 0.5f),
        polygon(x - 0.5f, y - 0.5f, x - 0.5f, y + 0.5f, x + 0.5f, y + 0.5f)
    };
}

tp_map_importer::tp_map_importer(map& m)
: m(m)
{}

int tp_map_importer::tp_import(
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

    m.is_loaded = true;

    return 0;
}

void tp_map_importer::tp_toggle_ref(
    const std::uint32_t x,
    const std::uint32_t y,
    const std::size_t id,
    const toggle_tag_type type
) {
    // see if this is referenced by a toggle
    const tp_pos toggle_check(x, y);
    if(tp_import_toggle_positions.find(toggle_check)
    != tp_import_toggle_positions.end()) {
        for(const auto t : tp_import_toggle_positions[toggle_check]) {
            auto && tags = m.toggles[t]->tags;
            tags.emplace_back(id, type);
        }
    }
}

int tp_map_importer::tp_import_json(const std::string & src)
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
        {"none-gravityCTF", map_type::none_gravityCTF},
        {"gravity",         map_type::gravity},
        {"gravityCTF",      map_type::gravityCTF},
    };
    if(j.find("info") == j.end()) {
        std::cerr << "error: 'info' not found" << std::endl;
        return 1;
    }
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

    m.type    = maptype_map[j_maptype];
    if(j_info.find("name") == j_info.end()) {
        m.name = "unknown name";
    } else {
        m.name = j_info.at("name");
    }

    if(j_info.find("author") == j_info.end()) {
        m.author = "unknown-author";
    } else {
        m.author = j_info.at("author");
    }
    m.version = 1;
    switch(m.type) {
    case map_type::none_gravityCTF:
    case map_type::gravity:
    case map_type::gravityCTF:
        m.gravity = 9.8; // todo is this correct value?
        m.jumping_enabled = true;
        break;
    default:
        m.gravity = 0.0;
        m.jumping_enabled = false;
        break;
    }

    if(j.find("switches") != j.end()) {
        auto j_toggle = j.at("switches");
        for (json::iterator it = j_toggle.begin(); it != j_toggle.end(); ++it) {
            if(it.key() == "timer") {
                continue;
            }

            std::vector<std::string> pieces = split_on(it.key(), ',');
            if(pieces.size() != 2) {
                continue;
            }
            const std::uint32_t x = std::stoi(pieces.at(0));
            const std::uint32_t y = std::stoi(pieces.at(1));
            
            const std::uint32_t timer = (it.value().find("timer") != it.value().end())
                ? it.value()["timer"].get<int>()
                : 0;

            m.toggles.emplace_back(new toggle(x, y, timer, {}));

            for(auto p : it.value().at("toggle")) {
                const auto j_pos = p.at("pos");
                tp_import_toggle_positions[
                    tp_pos(j_pos.at("x"), j_pos.at("y"))
                ].emplace_back(m.toggles.size() - 1);
            }

            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p, config.COLOR_TILE, tile_type::normal));
            }
        }
    }

    if(j.find("portals") != j.end()) {
        auto j_portals = j.at("portals");

        std::map<tp_pos, portal> destination_portals;

        for (json::iterator it = j_portals.begin(); it != j_portals.end(); ++it) {
            const auto j_port = it.value();

            std::vector<std::string> pieces = split_on(it.key(), ',');
            if(pieces.size() != 2) {
                continue;
            }
            const std::uint32_t x = std::stoi(pieces.at(0));
            const std::uint32_t y = std::stoi(pieces.at(1));

            portal p(x, y);

            if(j_port.find("cooldown") != j_port.end()) {
                p.set_cooldown(j_port.at("cooldown"));
            }

            if(j_port.find("destination") != j_port.end()) {
                auto j_dest = j_port.at("destination");
                if(j_dest.find("x") != j_dest.end() && j_dest.find("y") != j_dest.end()) {
                    p.has_destination = true;
                    const tp_pos dpos(j_dest.at("x"), j_dest.at("y"));
                    destination_portals[dpos] = p;
                }
            }

            m.portals.emplace_back(new portal(p));

            // save position into map
            // so we can reference this same map to find destination id
            // from x,y coords
            tp_import_portal_positions[tp_pos(x, y)] = m.portals.size() - 1;
        }

        // add portals from destination_portals list which only exist as destinations
        for(auto iter=destination_portals.begin(); iter != destination_portals.end(); ++iter) {
            const tp_pos dpos = (*iter).first;

            if(tp_import_portal_positions.find(dpos) == tp_import_portal_positions.end()) {
                m.portals.emplace_back(new portal(dpos.x, dpos.y));
                tp_import_portal_positions[dpos] = m.portals.size() - 1;
            }
        }

        // we loop through again to finish adding the destinations as ids
        std::size_t id = 0;
        for (json::iterator it = j_portals.begin(); it != j_portals.end(); ++it, ++id) {
            const auto j_port = it.value();
            if(id >= m.portals.size()) {
                std::cerr << "portal id generation error" << std::endl;
                return 1;
            }
            portal* p = m.portals.at(id).get();

            if(p->has_destination) {
                auto j_dest = j_port.at("destination");
                const tp_pos dpos(j_dest.at("x"), j_dest.at("y"));

                if(tp_import_portal_positions.find(dpos) == tp_import_portal_positions.end()) {
                    spdlog::get("game")->info("tp_import_portal_position at {0:d},{0:d} not found", dpos.x, dpos.y);
                    p->has_destination = false;
                    continue;
                }
                const std::size_t destination_id = tp_import_portal_positions.at(dpos);
                p->destination_id = destination_id;
            }
        }
    }

    if(j.find("fields") != j.end()) {
        auto j_fields = j.at("fields");
        for (json::iterator it = j_fields.begin(); it != j_fields.end(); ++it) {
            std::vector<std::string> pieces = split_on(it.key(), ',');
            if(pieces.size() != 2) {
                continue;
            }
            const std::uint32_t x = std::stoi(pieces.at(0));
            const std::uint32_t y = std::stoi(pieces.at(1));

            std::map<std::string, gate_type> gatetype_map = {
                {"off",  gate_type::off},
                {"on",   gate_type::green},
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
                m.gates.emplace_back(new gate(p, type));
                tp_toggle_ref(x, y, m.gates.size()-1, toggle_tag_type::gate);
            }
        }
    }

    if(j.find("spawnPoints") != j.end()) {
        const auto j_spawnpoints = j.at("spawnPoints");
        auto load_spawnpoints = [&](const spawn_type type) {
            auto j_spawns = (type == spawn_type::blue)
                ? j_spawnpoints.at("blue")
                : j_spawnpoints.at("red");

            for (json::iterator it = j_spawns.begin(); it != j_spawns.end(); ++it) {
                const std::uint32_t x      = it.value()["x"];
                const std::uint32_t y      = it.value()["y"];
                const std::uint32_t radius = (it.value().find("radius") != it.value().end())
                    ? static_cast<float>(it.value()["radius"])
                    : 1.0;
                const std::uint32_t weight = (it.value().find("weight") != it.value().end())
                    ? static_cast<std::uint32_t>(it.value()["weight"])
                    : 1;

                m.spawns.emplace_back(new spawn(x, y, radius, weight, type));
            }
        };

        if(j_spawnpoints.find("red") != j_spawnpoints.end()) {
            load_spawnpoints(spawn_type::red);
        }
        if(j_spawnpoints.find("blue") != j_spawnpoints.end()) {
            load_spawnpoints(spawn_type::blue);
        }
    }

    return 0;
}

int tp_map_importer::tp_import_png(const std::string & src)
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

    m.width  = w;
    m.height = h;

    std::map<std::string, tp_tile_type> tp_tiletype_map  = {
        {"000000", tp_tile_type::background},
        {"d4d4d4", tp_tile_type::tile},
        {"dcbaba", tp_tile_type::speed_red},
        {"bbb8dd", tp_tile_type::speed_blue},
        {"dcdcba", tp_tile_type::speed_yellow},
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
        {"ffff00", tp_tile_type::booster_all},
        {"ff7373", tp_tile_type::booster_red},
        {"7373ff", tp_tile_type::booster_blue},
        {"007500", tp_tile_type::gate},
        {"cac000", tp_tile_type::portal},
        {"808000", tp_tile_type::flag_neutral},
        {"ff0000", tp_tile_type::flag_red},
        {"0000ff", tp_tile_type::flag_blue},
        {"9b0000", tp_tile_type::respawn_red},
        {"00009b", tp_tile_type::respawn_blue},
        {"202020", tp_tile_type::gravitywell},
        {"ff7373", tp_tile_type::marsball},
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
                m.tiles.emplace_back(new tile(p, col, tile_type::background));
            }
        } else if(tiletype == tp_tile_type::tile) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p, col, tile_type::normal));
            }
        } else if(tiletype == tp_tile_type::speed_red) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p, col, tile_type::speed_red));
            }
        } else if(tiletype == tp_tile_type::speed_blue) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p, col, tile_type::speed_blue));
            }
        } else if(tiletype == tp_tile_type::speed_yellow) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p, col, tile_type::speed_yellow));
            }
        } else if(tiletype == tp_tile_type::endzone_red) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p, col, tile_type::endzone_red));
            }
        } else if(tiletype == tp_tile_type::endzone_blue) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p, col, tile_type::endzone_blue));
            }
        } else if(tiletype == tp_tile_type::wall) {
            for(auto p : make_square_poly(x, y)) {
                m.walls.emplace_back(new wall(p, config.COLOR_WALL));
            }
        } else if(tiletype == tp_tile_type::wall_tl) {
            m.walls.emplace_back(new wall(polygon(x-0.5, y-0.5, x+0.5, y-0.5, x-0.5, y+0.5), config.COLOR_WALL));
            m.tiles.emplace_back(new tile(polygon(x+0.5, y-0.5, x+0.5, y+0.5, x-0.5, y+0.5), config.COLOR_TILE, tile_type::normal));
        } else if(tiletype == tp_tile_type::wall_tr) {
            m.walls.emplace_back(new wall(polygon(x-0.5, y-0.5, x+0.5, y-0.5, x+0.5, y+0.5), config.COLOR_WALL));
            m.tiles.emplace_back(new tile(polygon(x-0.5, y-0.5, x+0.5, y+0.5, x-0.5, y+0.5), config.COLOR_TILE, tile_type::normal));
        } else if(tiletype == tp_tile_type::wall_bl) {
            m.walls.emplace_back(new wall(polygon(x-0.5, y-0.5, x+0.5, y+0.5, x-0.5, y+0.5), config.COLOR_WALL));
            m.tiles.emplace_back(new tile(polygon(x-0.5, y-0.5, x+0.5, y-0.5, x+0.5, y+0.5), config.COLOR_TILE, tile_type::normal));
        } else if(tiletype == tp_tile_type::wall_br) {
            m.walls.emplace_back(new wall(polygon(x+0.5, y-0.5, x+0.5, y+0.5, x-0.5, y+0.5), config.COLOR_WALL));
            m.tiles.emplace_back(new tile(polygon(x-0.5, y-0.5, x+0.5, y-0.5, x-0.5, y+0.5), config.COLOR_TILE, tile_type::normal));
        } else if(tiletype == tp_tile_type::bomb) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.bombs.emplace_back(new bomb(x, y));
            tp_toggle_ref(x, y, m.bombs.size()-1, toggle_tag_type::bomb);
        } else if(tiletype == tp_tile_type::spike) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.spikes.emplace_back(new spike(x, y));
        } else if(tiletype == tp_tile_type::powerup) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.powerups.emplace_back(new powerup(x, y, {
                powerup_type::tagpro,
                powerup_type::jukejuice,
                powerup_type::rollingbomb
            }));
        } else if(tiletype == tp_tile_type::button) {
            // this is handled by toggle
        } else if(tiletype == tp_tile_type::booster_all) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.boosters.emplace_back(new booster(x, y, booster_type::all));
        } else if(tiletype == tp_tile_type::booster_red) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.boosters.emplace_back(new booster(x, y, booster_type::red));
        } else if(tiletype == tp_tile_type::booster_blue) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.boosters.emplace_back(new booster(x, y, booster_type::blue));
        } else if(tiletype == tp_tile_type::gate) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
        } else if(tiletype == tp_tile_type::portal) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
        } else if(tiletype == tp_tile_type::flag_neutral) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.flags.emplace_back(new flag(x, y, flag_type::neutral));
        } else if(tiletype == tp_tile_type::flag_red) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.flags.emplace_back(new flag(x, y, flag_type::red));
        } else if(tiletype == tp_tile_type::flag_blue) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.flags.emplace_back(new flag(x, y, flag_type::blue));
        } else if(tiletype == tp_tile_type::respawn_red) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.spawns.emplace_back(new spawn(x, y, 1.0, 1.0, spawn_type::red));
        } else if(tiletype == tp_tile_type::respawn_blue) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }
            m.spawns.emplace_back(new spawn(x, y, 1.0, 1.0, spawn_type::blue));
        } else if(tiletype == tp_tile_type::gravitywell) {
            for(auto p : make_square_poly(x, y)) {
                m.tiles.emplace_back(new tile(p,
                                              config.COLOR_TILE,
                                              tile_type::normal));
            }

            m.gravwells.emplace_back(new gravwell(x, y, 6.5, 24.0)); // todo -- set correct force
        } else if(tiletype == tp_tile_type::marsball) {
            std::cerr << "error: marsball not supported" << std::endl;
            return 1;
        } else {
            std::cerr << "error: unhandled tile_type: " << tile_color << std::endl;
            return 1;
        }
    }

    /* could do this as polys are added but keeping
     * it isolated for now
     */
    std::vector<polygon> poly_set;
    for(const auto& w : m.walls) {
        poly_set.push_back(w->poly);
    }
    m.chains = poly2chain(poly_set);

    return 0;
}

