#include "server_event.hpp"

server_event::server_event(server_event_chat e)
: type(server_event_type::chat)
, ptr(new server_event_chat(e))
{}

server_event::server_event(server_event_teamchat e)
: type(server_event_type::teamchat)
, ptr(new server_event_teamchat(e))
{}

server_event::server_event(server_event_player_joined e)
: type(server_event_type::player_joined)
, ptr(new server_event_player_joined(e))
{}

server_event::server_event(server_event_player_left e)
: type(server_event_type::player_left)
, ptr(new server_event_player_left(e))
{}

server_event::server_event(server_event_movement e)
: type(server_event_type::movement)
, ptr(new server_event_movement(e))
{}

server_event::server_event(server_event_honk e)
: type(server_event_type::honk)
, ptr(new server_event_honk(e))
{}

server_event::server_event(server_event_ballsync e)
: type(server_event_type::ballsync)
, ptr(new server_event_ballsync(e))
{}

server_event::server_event(server_event_ball_respawn e)
: type(server_event_type::ball_respawn)
, ptr(new server_event_ball_respawn(e))
{}

server_event::server_event(server_event_bomb_respawn e)
: type(server_event_type::bomb_respawn)
, ptr(new server_event_bomb_respawn(e))
{}

server_event::server_event(server_event_bomb_explosion e)
: type(server_event_type::bomb_explosion)
, ptr(new server_event_bomb_explosion(e))
{}

server_event::server_event(server_event_powerup_respawn e)
: type(server_event_type::powerup_respawn)
, ptr(new server_event_powerup_respawn(e))
{}

server_event::server_event(server_event_booster_respawn e)
: type(server_event_type::booster_respawn)
, ptr(new server_event_booster_respawn(e))
{}

server_event::server_event(server_event_portal_respawn e)
: type(server_event_type::portal_respawn)
, ptr(new server_event_portal_respawn(e))
{}

server_event::server_event(server_event_ball_popped e)
: type(server_event_type::ball_popped)
, ptr(new server_event_ball_popped(e))
{}

server_event::server_event(server_event_ball_boosted e)
: type(server_event_type::ball_boosted)
, ptr(new server_event_ball_boosted(e))
{}
