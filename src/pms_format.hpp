#ifndef ML_PMS_LOADER_HPP
#define ML_PMS_LOADER_HPP

#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

namespace pms {

template<class T> T read_bin(std::istream& is)
{
    T tmp;
    is.read(reinterpret_cast<char*>( &tmp ), sizeof(T));
    return tmp;
}

std::string read_string(std::istream& is, const std::size_t full_length);

enum POLYTYPE
{
    ptNORMAL = 0,
    ptONLY_BULLETS_COLLIDE,
    ptONLY_PLAYERS_COLLIDE,
    ptNO_COLLIDE,
    ptICE,
    ptDEADLY,
    ptBLOODY_DEADLY,
    ptHURTS,
    ptREGENERATES,
    ptLAVA
};

enum DRAWBEHIND
{
    dbBEHIND_ALL = 0,
    dbBEHIND_MAP,
    dbBEHIND_NONE
};

enum SPECIALACTIONS
{
    saNONE = 0,
    saSTOP_AND_CAMP,
    saWAIT_1_SECOND,
    saWAIT_5_SECONDS,
    saWAIT_10_SECONDS,
    saWAIT_15_SECONDS,
    saWAIT_20_SECONDS
};

enum WEATHERTYPE
{
    wtNONE = 0,
    wtRAIN,
    wtSANDSTORM,
    wtSNOW
};

enum STEPSTYPE
{
    stHARD_GROUND = 0,
    stSOFT_GROUND,
    stNONE
};

enum SPAWNTEAM
{
    stGENERAL = 0,
    stALPHA,
    stBRAVO,
    stCHARLIE,
    stDELTA,
    stALPHA_FLAG,
    stBRAVO_FLAG,
    stGRENADES,
    stMEDKITS,
    stCLUSTERS,
    stVEST,
    stFLAMER,
    stBERSERKER,
    stPREDATOR,
    stYELLOW_FLAG,
    stRAMBO_BOW,
    stSTAT_GUN
};

struct vec3
{
    float x;
    float y;
    float z;
};

struct color
{
    std::uint8_t blue;
    std::uint8_t green;
    std::uint8_t red;
    std::uint8_t alpha;
};

// more stuff

struct vertex
{
    float x;
    float y;
    float z;
    float rhw;
    color col;
    float tu;
    float tv;
};

struct polygon
{
    vertex vertex[3];
    vec3 perpendicular[3];
    POLYTYPE polyType;
};

struct sector
{
    std::vector<std::uint16_t> polys;
};

struct prop
{
    bool active;
    std::uint8_t filler1;
    std::uint16_t style;
    std::int32_t width;
    std::int32_t height;
    float x;
    float y;
    float rotation;
    float scaleX;
    float scaleY;
    std::uint8_t alpha;
    std::uint8_t filler2[3];
    color col;
    DRAWBEHIND level;
    std::uint8_t filler3[3];
};

struct dostime
{
    std::uint16_t second : 5;
    std::uint16_t minute : 6;
    std::uint16_t hour : 5;
};

struct dosdate
{
    std::uint16_t day : 5;
    std::uint16_t month : 4;
    std::uint16_t year : 7;
};

struct timestamp
{
    dostime time;
    dosdate date;
};

struct scenery
{
    std::string name;
    timestamp ts;
};

struct collider
{
    bool active;
    std::uint8_t filler[3];
    float x;
    float y;
    float radius;
};

struct spawnpoint
{
    bool active;
    std::uint8_t filler[3];
    std::int32_t x;
    std::int32_t y;
    SPAWNTEAM team;
};

struct waypoint
{
    bool active;
    std::uint8_t filler1[3];
    std::int32_t id;
    std::int32_t x;
    std::int32_t y;
    bool left;
    bool right;
    bool up;
    bool down;
    bool jet;
    std::uint8_t path;
    SPECIALACTIONS specialAction;
    std::uint8_t c2;
    std::uint8_t c3;
    std::uint8_t filler2[3];
    std::int32_t numConnections;
    std::int32_t connections[20];
};

// Pms structure

struct pms
{
    std::int32_t version;
    std::string name;
    std::string texture;
    std::int32_t jetAmount;
    WEATHERTYPE weather;
    std::vector<polygon> polygons;
    std::vector<sector> sectors;
    std::vector<prop> props;
    std::vector<scenery> scenerys;
    std::vector<collider> colliders;
    std::vector<spawnpoint> spawnpoints;
    std::vector<waypoint> waypoints;
    float leftoffs;
    float rightoffs;
    float topoffs;
    float bottomoffs;
};

// Reader

std::istream& operator>>(std::istream& is, pms& p);

}

#endif
