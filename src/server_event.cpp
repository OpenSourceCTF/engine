#include "server_event.hpp"

server_event::server_event(server_event_gamesync e)
: type(server_event_type::gamesync)
, ptr(new server_event_gamesync(e))
{}

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

server_event::server_event(server_event_ball_score e)
: type(server_event_type::ball_score)
, ptr(new server_event_ball_score(e))
{}

server_event::server_event(server_event_ball_portal e)
: type(server_event_type::ball_portal)
, ptr(new server_event_ball_portal(e))
{}

server_event::server_event(server_event_ball_powerup e)
: type(server_event_type::ball_powerup)
, ptr(new server_event_ball_powerup(e))
{}

server_event::server_event(server_event_ball_rb_explode e)
: type(server_event_type::ball_rb_explode)
, ptr(new server_event_ball_rb_explode(e))
{}

server_event::server_event(server_event_flag_grabbed e)
: type(server_event_type::flag_grabbed)
, ptr(new server_event_flag_grabbed(e))
{}

server_event::server_event(server_event_toggle_on e)
: type(server_event_type::toggle_on)
, ptr(new server_event_toggle_on(e))
{}

server_event::server_event(server_event_toggle_off e)
: type(server_event_type::toggle_off)
, ptr(new server_event_toggle_off(e))
{}

server_event::server_event(server_event_game_finished e)
: type(server_event_type::game_finished)
, ptr(new server_event_game_finished(e))
{}

server_event::server_event(server_event_game_stats e)
: type(server_event_type::game_stats)
, ptr(new server_event_game_stats(e))
{}

server_event::server_event(server_event_vote_player e)
: type(server_event_type::vote_player)
, ptr(new server_event_vote_player(e))
{}


