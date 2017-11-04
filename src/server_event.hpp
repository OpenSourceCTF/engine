#ifndef ML_SERVER_EVENT_HPP
#define ML_SERVER_EVENT_HPP

#include "server_event_type.hpp"
#include "server_event_player_joined.hpp"
#include "server_event_player_left.hpp"
#include "server_event_chat.hpp"
#include "server_event_teamchat.hpp"
#include "server_event_movement.hpp"
#include "server_event_honk.hpp"
#include "server_event_ballsync.hpp"
#include "server_event_ball_respawn.hpp"
#include "server_event_bomb_respawn.hpp"
#include "server_event_bomb_explosion.hpp"
#include "server_event_powerup_respawn.hpp"
#include "server_event_booster_respawn.hpp"
#include "server_event_portal_respawn.hpp"
#include "server_event_ball_popped.hpp"
#include "server_event_ball_boosted.hpp"
#include "server_event_ball_score.hpp"
#include "server_event_flag_grabbed.hpp"

struct server_event_chat;
struct server_event_teamchat;
struct server_event_player_joined;
struct server_event_player_left;
struct server_event_movement;
struct server_event_honk;
struct server_event_ballsync;
struct server_event_ball_respawn;
struct server_event_bomb_respawn;
struct server_event_bomb_explosion;
struct server_event_powerup_respawn;
struct server_event_booster_respawn;
struct server_event_portal_respawn;
struct server_event_ball_popped;
struct server_event_ball_boosted;
struct server_event_ball_score;
struct server_event_flag_grabbed;

struct server_event
{
    server_event_type type;
    void* ptr;

    server_event(server_event_chat e);
    server_event(server_event_teamchat e);
    server_event(server_event_player_joined e);
    server_event(server_event_player_left e);
    server_event(server_event_movement e);
    server_event(server_event_honk e);
    server_event(server_event_ballsync e);
    server_event(server_event_ball_respawn e);
    server_event(server_event_bomb_respawn e);
    server_event(server_event_bomb_explosion e);
    server_event(server_event_powerup_respawn e);
    server_event(server_event_booster_respawn e);
    server_event(server_event_portal_respawn e);
    server_event(server_event_ball_popped e);
    server_event(server_event_ball_boosted e);
    server_event(server_event_ball_score e);
    server_event(server_event_flag_grabbed e);
};

#endif

