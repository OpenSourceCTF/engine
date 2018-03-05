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
    is.read( reinterpret_cast<char*>( &tmp ), sizeof(T));
    return tmp;
}

std::string read_string(std::istream& is, int full_length);

typedef enum POLYTYPE
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
} PMS_POLYTYPE;

typedef enum DRAWBEHIND
{
    dbBEHIND_ALL = 0,
    dbBEHIND_MAP,
    dbBEHIND_NONE
} PMS_DRAWBEHIND;

typedef enum SPECIALACTIONS
{
    saNONE = 0,
    saSTOP_AND_CAMP,
    saWAIT_1_SECOND,
    saWAIT_5_SECONDS,
    saWAIT_10_SECONDS,
    saWAIT_15_SECONDS,
    saWAIT_20_SECONDS
} PMS_SPECIALACTIONS;

typedef enum WEATHERTYPE
{
    wtNONE = 0,
    wtRAIN,
    wtSANDSTORM,
    wtSNOW
} PMS_WEATHERTYPE;

typedef enum STEPSTYPE
{
    stHARD_GROUND = 0,
    stSOFT_GROUND,
    stNONE
} PMS_STEPSTYPE;

typedef enum SPAWNTEAM
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
} PMS_SPAWNTEAM;

typedef struct tagPMS_VECTOR
{
    float x;
    float y;
    float z;
} PMS_VECTOR;

typedef struct tagPMS_COLOR
{
    std::uint8_t blue;
    std::uint8_t green;
    std::uint8_t red;
    std::uint8_t alpha;
} PMS_COLOR;

// more stuff

typedef struct tagPMS_VERTEX
{
    float x;
    float y;
    float z;
    float rhw;
    PMS_COLOR color;
    float tu;
    float tv;
} PMS_VERTEX;

typedef struct tagPMS_POLYGON
{
    PMS_VERTEX vertex[3];
    PMS_VECTOR perpendicular[3];
    PMS_POLYTYPE polyType;
} PMS_POLYGON;

typedef struct tagPMS_SECTOR
{
    std::uint16_t polyCount;
    std::vector<std::uint16_t> polys;
} PMS_SECTOR;

typedef struct tagPMS_PROP
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
    PMS_COLOR color;
    PMS_DRAWBEHIND level;
    std::uint8_t filler3[3];
} PMS_PROP;

typedef struct tagDOSTIME
{
    std::uint16_t second : 5;
    std::uint16_t minute : 6;
    std::uint16_t hour : 5;
} DOSTIME;

typedef struct tagDOSDATE
{
    std::uint16_t day : 5;
    std::uint16_t month : 4;
    std::uint16_t year : 7;
} DOSDATE;

typedef struct tagPMS_TIMESTAMP
{
    DOSTIME time;
    DOSDATE date;
} PMS_TIMESTAMP;

typedef struct tagPMS_SCENERY
{
    std::string name;
    PMS_TIMESTAMP timestamp;
} PMS_SCENERY;

typedef struct tagPMS_COLLIDER
{
    bool active;
    std::uint8_t filler[3];
    float x;
    float y;
    float radius;
} PMS_COLLIDER;

typedef struct tagPMS_SPAWNPOINT
{
    bool active;
    std::uint8_t filler[3];
    std::int32_t x;
    std::int32_t y;
    PMS_SPAWNTEAM team;
} PMS_SPAWNPOINT;

typedef struct tagPMS_WAYPOINT
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
    PMS_SPECIALACTIONS specialAction;
    std::uint8_t c2;
    std::uint8_t c3;
    std::uint8_t filler2[3];
    std::int32_t numConnections;
    std::int32_t connections[20];
} PMS_WAYPOINT;

// Pms structure

struct pms
{
    std::int32_t version;
    std::string name;
    std::string texture;
    PMS_COLOR bgColorTop;
    PMS_COLOR bgColorBottom;
    std::int32_t jetAmount;
    std::uint8_t grenades;
    std::uint8_t medikits;
    PMS_WEATHERTYPE weather;
    PMS_STEPSTYPE steps;
    std::int32_t randID;
    std::int32_t polygonCount;
    std::vector<PMS_POLYGON> polygon;
    std::int32_t sectorDivisions;
    std::int32_t numSectors;
    std::vector<PMS_SECTOR> sector;
    std::int32_t propCount;
    std::vector<PMS_PROP> prop;
    std::int32_t sceneryCount;
    std::vector<PMS_SCENERY> scenery;
    std::int32_t colliderCount;
    std::vector<PMS_COLLIDER> collider;
    std::int32_t spawnpointCount;
    std::vector<PMS_SPAWNPOINT> spawnpoint;
    std::int32_t waypointCount;
    std::vector<PMS_WAYPOINT> waypoint;
    float leftoffs;
    float rightoffs;
    float topoffs;
    float bottomoffs;
};

// Reader

std::istream& operator>>(std::istream& is, pms& p);

}

#endif
