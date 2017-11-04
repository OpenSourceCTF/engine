#include "game.hpp"

game_accessor::game_accessor()
: g(nullptr)
{}

game_accessor::game_accessor(game* g)
: g(g)
{}

void game_accessor::set_game(game* g)
{
    this->g = g;
}

void game_accessor::add_server_event(server_event && e)
{
    g->add_server_event(e);
}

game_accessor::operator game*()
{
    return g;
}
