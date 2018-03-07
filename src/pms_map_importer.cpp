#include "pms_map_importer.hpp"

pms_map_importer::pms_map_importer(map& m)
: m(m)
{}

int pms_map_importer::import(
    const std::string & src,
    const double scale_size
) {
    const settings& config = settings::get_instance();

    std::ifstream inputfile_stream(src);
    if(! inputfile_stream.is_open()) {
        std::cerr << "error: unable to load " << src << std::endl;
        return -1;
    }

    std::ifstream mapf(src, std::ios::binary);
    pms::pms map;
    mapf >> map;
    mapf.close();


    m.type = map_type::normal;
    m.name = map.name;
    m.author = "exported from PMS";
    m.version = map.version;
    m.gravity = 0;
    m.jumping_enabled = false;

    for(pms::polygon o : map.polygons) {
        const auto type = o.polyType;
        bool is_tile = false;
        switch(type) {
            case pms::ptONLY_BULLETS_COLLIDE:
            case pms::ptNO_COLLIDE:
                is_tile = true;
                break;
            default: break;
        }
        const b2Vec2 v1(
            o.vertex[0].x * scale_size,
            o.vertex[0].y * scale_size
        );
        const b2Vec2 v2(
            o.vertex[1].x * scale_size,
            o.vertex[1].y * scale_size
        );
        const b2Vec2 v3(
            o.vertex[2].x * scale_size,
            o.vertex[2].y * scale_size
        );

        if(is_tile) {
            m.tiles.emplace_back(new tile(polygon(v1, v2, v3, config.COLOR_TILE), tile_type::normal));
        } else {
            const color c1(o.vertex[0].col.red, o.vertex[0].col.green, o.vertex[0].col.blue, o.vertex[0].col.alpha);
            const color c2(o.vertex[1].col.red, o.vertex[1].col.green, o.vertex[1].col.blue, o.vertex[1].col.alpha);
            const color c3(o.vertex[2].col.red, o.vertex[2].col.green, o.vertex[2].col.blue, o.vertex[2].col.alpha);
            m.walls.emplace_back(new wall(polygon(v1, v2, v3, c1, c2, c3)));
        }
    }

    for(pms::spawnpoint o : map.spawnpoints) {
        const float x = o.x * scale_size;
        const float y = o.y * scale_size;

        switch(o.team) {
            case pms::stALPHA:
                m.spawns.emplace_back(new spawn(x, y, 1.0, 1.0, spawn_type::red));
                break;
            case pms::stBRAVO:
                m.spawns.emplace_back(new spawn(x, y, 1.0, 1.0, spawn_type::blue));
                break;
            case pms::stALPHA_FLAG:
                m.flags.emplace_back(new flag(x, y, flag_type::red));
                break;
            case pms::stBRAVO_FLAG:
                m.flags.emplace_back(new flag(x, y, flag_type::blue));
                break;
            case pms::stYELLOW_FLAG:
                m.flags.emplace_back(new flag(x, y, flag_type::neutral));
                break;
            case pms::stGRENADES:
            case pms::stMEDKITS:
                m.boosters.emplace_back(new booster(x, y, booster_type::all));
                break;
            case pms::stCLUSTERS:
            case pms::stVEST:
            case pms::stFLAMER:
            case pms::stBERSERKER:
            case pms::stPREDATOR:
                m.powerups.emplace_back(new powerup(x, y, {
                    powerup_type::jukejuice,
                    powerup_type::rollingbomb,
                    powerup_type::tagpro,
                }));
                break;
            default: continue;
        }
    }

    // todo should this and other in tp_map_importer be moved somewhere else?
    /* could do this as polys are added but keeping
     * it isolated for now
     */
    std::vector<polygon> poly_set;
    for(const auto& w : m.walls) {
        poly_set.push_back(w->poly);
    }
    m.chains = poly2chain(poly_set);

    m.is_loaded = true;
    return 0;
}

