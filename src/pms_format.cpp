#include "pms_format.hpp"

namespace pms {

std::string read_string(std::istream& is, const std::size_t full_length)
{
    int length = read_bin<char>(is);
    std::vector<char> tmp(full_length);
    is.read( &tmp[0], tmp.size() );
    return std::string(tmp.begin(), tmp.begin() + length);
}

std::istream& operator>>(std::istream& is, pms& p)
{
    p.version = read_bin<int>(is);
    p.name = read_string(is, 38);
    p.texture = read_string(is, 24);
    const color bgColorTop = read_bin<color>(is);
    const color bgColorBottom = read_bin<color>(is);
    const std::int32_t jetAmount = read_bin<std::int32_t>(is);
    const std::uint8_t grenades = read_bin<std::uint8_t>(is);
    const std::uint8_t medikits = read_bin<std::uint8_t>(is);
    const WEATHERTYPE weather = static_cast<WEATHERTYPE>(read_bin<std::uint8_t>(is));
    const STEPSTYPE steps = static_cast<STEPSTYPE>(read_bin<std::uint8_t>(is));
    const std::int32_t randID = read_bin<std::int32_t>(is);

    const std::size_t polygonCount = read_bin<std::int32_t>(is);
    for (std::size_t i=0; i<polygonCount; ++i) {
        polygon temp; // cheap solution
        temp.vertex[0] = read_bin<vertex>(is);
        temp.vertex[1] = read_bin<vertex>(is);
        temp.vertex[2] = read_bin<vertex>(is);
        temp.perpendicular[0] = read_bin<vec3>(is);
        temp.perpendicular[1] = read_bin<vec3>(is);
        temp.perpendicular[2] = read_bin<vec3>(is);
        temp.polyType = POLYTYPE(read_bin<std::uint8_t>(is));
        p.polygons.emplace_back(temp);
    }

    {
        const std::size_t sectorDivisions = read_bin<std::int32_t>(is);
        const std::size_t numSectors = read_bin<std::int32_t>(is);
        p.topoffs    = sectorDivisions * -numSectors;
        p.bottomoffs = sectorDivisions *  numSectors;
        p.leftoffs   = sectorDivisions * -numSectors;
        p.rightoffs  = sectorDivisions *  numSectors;

        for(std::size_t i=0; i<((numSectors*2)+1)*((numSectors*2)+1); ++i) {
            sector temp;

            const std::size_t polyCount = read_bin<std::uint16_t>(is);
            for(std::size_t m = 0; m<polyCount; ++m) {
                temp.polys.emplace_back(read_bin<std::uint16_t>(is));
            }

            p.sectors.emplace_back(temp);
        }
    }

    const std::size_t propCount = read_bin<std::int32_t>(is);
    for(std::size_t i=0; i<propCount; ++i) {
        prop temp;
        temp.active = bool(read_bin<std::uint8_t>(is));
        temp.filler1 = read_bin<std::uint8_t>(is);
        temp.style = read_bin<std::uint16_t>(is);
        temp.width = read_bin<std::int32_t>(is);
        temp.height = read_bin<std::int32_t>(is);
        temp.x = read_bin<float>(is);
        temp.y = read_bin<float>(is);
        temp.rotation = read_bin<float>(is);
        temp.scaleX = read_bin<float>(is);
        temp.scaleY = read_bin<float>(is);
        temp.alpha = read_bin<std::uint8_t>(is);
        temp.filler2[0] = read_bin<std::uint8_t>(is);
        temp.filler2[1] = read_bin<std::uint8_t>(is);
        temp.filler2[2] = read_bin<std::uint8_t>(is);
        temp.col = read_bin<color>(is);
        temp.level = DRAWBEHIND(read_bin<std::uint8_t>(is));
        temp.filler3[0] = read_bin<std::uint8_t>(is);
        temp.filler3[1] = read_bin<std::uint8_t>(is);
        temp.filler3[2] = read_bin<std::uint8_t>(is);
        p.props.emplace_back(temp);
    }

    const std::size_t sceneryCount = read_bin<std::int32_t>(is);
    for(std::size_t i=0; i<sceneryCount; ++i) {
        scenery temp;
        //temp.nameLen = read_bin<std::uint8_t>( is );
        temp.name = read_string(is, 50);
        temp.ts = read_bin<timestamp>(is);
        p.scenerys.emplace_back(temp);
    }

    const std::size_t colliderCount = read_bin<std::int32_t>(is);
    for(std::size_t i=0; i<colliderCount; ++i) {
        collider temp;
        temp.active = bool(read_bin<std::uint8_t>(is));
        temp.filler[0] = read_bin<std::uint8_t>(is);
        temp.filler[1] = read_bin<std::uint8_t>(is);
        temp.filler[2] = read_bin<std::uint8_t>(is);
        temp.x = read_bin<float>(is);
        temp.y = read_bin<float>(is);
        temp.radius = read_bin<float>(is);
        p.colliders.emplace_back(temp);
    }

    const std::size_t spawnpointCount = read_bin<std::int32_t>(is);
    for(std::size_t i=0; i<spawnpointCount; ++i) {
        spawnpoint temp;
        temp.active = bool(read_bin<std::uint8_t>(is));
        temp.filler[0] = read_bin<std::uint8_t>(is);
        temp.filler[1] = read_bin<std::uint8_t>(is);
        temp.filler[2] = read_bin<std::uint8_t>(is);
        temp.x = read_bin<std::int32_t>(is);
        temp.y = read_bin<std::int32_t>(is);
        temp.team = SPAWNTEAM(read_bin<std::uint32_t>(is));
        p.spawnpoints.emplace_back(temp);
    }
    const std::size_t waypointCount = read_bin<std::int32_t>(is);
    for(std::size_t i = 0; i < waypointCount; ++i) {
        waypoint temp;
        temp.active = bool(read_bin<std::uint8_t>(is));
        temp.filler1[0] = read_bin<std::uint8_t>(is);
        temp.filler1[1] = read_bin<std::uint8_t>(is);
        temp.filler1[2] = read_bin<std::uint8_t>(is);
        temp.id = read_bin<std::int32_t>(is);
        temp.x = read_bin<std::int32_t>(is);
        temp.y = read_bin<std::int32_t>(is);
        temp.left = bool(read_bin<std::uint8_t>(is));
        temp.right = bool(read_bin<std::uint8_t>(is));
        temp.up = bool(read_bin<std::uint8_t>(is));
        temp.down = bool(read_bin<std::uint8_t>(is));
        temp.jet = bool(read_bin<std::uint8_t>(is));
        temp.path = read_bin<std::uint8_t>(is);
        temp.specialAction = SPECIALACTIONS(read_bin<std::uint8_t>(is));
        temp.c2 = read_bin<std::uint8_t>(is);
        temp.c3 = read_bin<std::uint8_t>(is);
        temp.filler2[0] = read_bin<std::uint8_t>(is);
        temp.filler2[1] = read_bin<std::uint8_t>(is);
        temp.filler2[2] = read_bin<std::uint8_t>(is);
        temp.numConnections = read_bin<std::int32_t>(is);
        for(std::size_t c=0; c<20; ++c) {
            temp.connections[c] = read_bin<std::int32_t>(is);
        }
        p.waypoints.emplace_back(temp);
    }
    return is;
}

}
