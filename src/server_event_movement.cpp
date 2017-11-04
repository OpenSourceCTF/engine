#include "server_event_movement.hpp"

server_event_movement::server_event_movement(
    player* p,
    int xdir,
    int ydir
)
: p(p)
, xdir(xdir)
, ydir(ydir)
{}
