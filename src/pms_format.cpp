#include "pms_format.hpp"

namespace pms {

std::string read_string(std::istream& is, int full_length)
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
	p.bgColorTop = read_bin<PMS_COLOR>(is);
	p.bgColorBottom = read_bin<PMS_COLOR>(is);
	p.jetAmount = read_bin<std::int32_t>(is);
	p.grenades = read_bin<std::uint8_t>(is);
	p.medikits = read_bin<std::uint8_t>(is);
	p.weather = PMS_WEATHERTYPE(read_bin<std::uint8_t>(is));
	p.steps = PMS_STEPSTYPE(read_bin<std::uint8_t>(is));
	p.randID = read_bin<std::int32_t>(is);
	p.polygonCount = read_bin<std::int32_t>(is);
	
	for (int i = 0; i<p.polygonCount; i++)
	{
		PMS_POLYGON temp; // cheap solution
		temp.vertex[0] = read_bin<PMS_VERTEX>(is);
		temp.vertex[1] = read_bin<PMS_VERTEX>(is);
		temp.vertex[2] = read_bin<PMS_VERTEX>(is);
		temp.perpendicular[0] = read_bin<PMS_VECTOR>(is);
		temp.perpendicular[1] = read_bin<PMS_VECTOR>(is);
		temp.perpendicular[2] = read_bin<PMS_VECTOR>(is);
		temp.polyType = PMS_POLYTYPE(read_bin<std::uint8_t>(is));
		p.polygon.push_back(temp);
	}
	p.sectorDivisions = read_bin<std::int32_t>(is);
	p.numSectors = read_bin<std::int32_t>(is);

	p.topoffs = p.sectorDivisions * -p.numSectors;
	p.bottomoffs = p.sectorDivisions * p.numSectors;
	p.leftoffs = p.sectorDivisions * -p.numSectors;
	p.rightoffs = p.sectorDivisions * p.numSectors;
	
	for (int i = 0; i< ((p.numSectors*2)+1)*((p.numSectors*2)+1); i++)
	{
		PMS_SECTOR temp;
		temp.polyCount = read_bin<std::uint16_t>(is);
		for (int m = 0; m<temp.polyCount; m++)
		{
			temp.polys.push_back(read_bin<std::uint16_t>(is));
		}
		p.sector.push_back(temp);
	}
	p.propCount = read_bin<std::int32_t>(is);
	for (int i = 0; i < p.propCount; i++)
	{
		PMS_PROP temp;
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
		temp.color = read_bin<PMS_COLOR>(is);
		temp.level = PMS_DRAWBEHIND(read_bin<std::uint8_t>(is));
		temp.filler3[0] = read_bin<std::uint8_t>(is);
		temp.filler3[1] = read_bin<std::uint8_t>(is);
		temp.filler3[2] = read_bin<std::uint8_t>(is);
		p.prop.push_back(temp);
	}
	p.sceneryCount = read_bin<std::int32_t>(is);
	for (int i = 0; i < p.sceneryCount; i++)
	{
		PMS_SCENERY temp;
		//temp.nameLen = read_bin<std::uint8_t>( is );
		temp.name = read_string(is, 50);
		temp.timestamp = read_bin<PMS_TIMESTAMP>(is);
		p.scenery.push_back(temp);
	}
	p.colliderCount = read_bin<std::int32_t>(is);
	for (int i = 0; i < p.colliderCount; i++)
	{
		PMS_COLLIDER temp;
		temp.active = bool(read_bin<std::uint8_t>(is));
		temp.filler[0] = read_bin<std::uint8_t>(is);
		temp.filler[1] = read_bin<std::uint8_t>(is);
		temp.filler[2] = read_bin<std::uint8_t>(is);
		temp.x = read_bin<float>(is);
		temp.y = read_bin<float>(is);
		temp.radius = read_bin<float>(is);
		p.collider.push_back(temp);
	}
	p.spawnpointCount = read_bin<std::int32_t>(is);
	for (int i = 0; i < p.spawnpointCount; i++)
	{
		PMS_SPAWNPOINT temp;
		temp.active = bool(read_bin<std::uint8_t>(is));
		temp.filler[0] = read_bin<std::uint8_t>(is);
		temp.filler[1] = read_bin<std::uint8_t>(is);
		temp.filler[2] = read_bin<std::uint8_t>(is);
		temp.x = read_bin<std::int32_t>(is);
		temp.y = read_bin<std::int32_t>(is);
		temp.team = PMS_SPAWNTEAM(read_bin<std::uint32_t>(is));
		p.spawnpoint.push_back(temp);
	}
	p.waypointCount = read_bin<std::int32_t>(is);
	for (int i = 0; i < p.waypointCount; i++)
	{
		PMS_WAYPOINT temp;
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
		temp.specialAction = PMS_SPECIALACTIONS(read_bin<std::uint8_t>(is));
		temp.c2 = read_bin<std::uint8_t>(is);
		temp.c3 = read_bin<std::uint8_t>(is);
		temp.filler2[0] = read_bin<std::uint8_t>(is);
		temp.filler2[1] = read_bin<std::uint8_t>(is);
		temp.filler2[2] = read_bin<std::uint8_t>(is);
		temp.numConnections = read_bin<std::int32_t>(is);
		for (int c=0; c<20; c++)
		{
			temp.connections[c] = read_bin<std::int32_t>(is);
		}
		p.waypoint.push_back(temp);
	}
	return is;
}

}
