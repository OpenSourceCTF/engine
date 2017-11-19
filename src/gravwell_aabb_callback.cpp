#include "gravwell_aabb_callback.hpp"

bool GravWellAABBCallback::ReportFixture(b2Fixture* fixture)
{
    if(fixture->GetBody()->GetMass() > 0)
    bodies.push_back(fixture->GetBody()); 
    return true; // continue
}


