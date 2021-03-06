#include "settings.hpp"

settings& settings::get_instance()
{
    static settings instance;

    if(instance.is_initialized) {
        return instance;
    }

    INIReader reader("config.ini");
    if(reader.ParseError() < 0) {
        spdlog::get("game")->error("cannot load 'config.ini'");
        return instance;
    } else {
        instance.BALL_MOVEMENT_SPEED   = reader.GetReal("ball", "movement_speed", 0);
        instance.BALL_SPEED_TILE_SPEED = reader.GetReal("ball", "speed_tile_speed", 0);
        instance.BALL_JUKEJUICE_SPEED  = reader.GetReal("ball", "jukejuice_speed", 0);
        instance.BALL_MAX_MOVEMENT_SPEED = reader.GetReal("ball", "max_movement_speed", 0);
        instance.BALL_JUMP_SPEED       = reader.GetReal("ball", "jump_speed", 0);
        instance.BALL_JUMP_MAX_V       = reader.GetReal("ball", "jump_max_v", 0);
        instance.BALL_JUMP_MAX_V_2     = reader.GetReal("ball", "jump_max_v_2", 0);
        instance.BALL_DAMPING          = reader.GetReal("ball", "damping", 0);
        instance.BALL_ANGULAR_DAMPING  = reader.GetReal("ball", "angular_damping", 0);
        instance.BALL_DENSITY          = reader.GetReal("ball", "density", 0);
        instance.BALL_FRICTION         = reader.GetReal("ball", "friction", 0);
        instance.BALL_RESTITUTION      = reader.GetReal("ball", "restitution", 0);
        instance.BALL_RADIUS           = reader.GetReal("ball", "radius", 0);
        instance.BALL_POP_FORCE        = reader.GetReal("ball","pop_force",0);
        instance.BALL_POP_RADIUS       = reader.GetReal("ball","pop_radius",0);
        instance.BALL_RESPAWN_TIME     = reader.GetInteger("ball", "respawn_time", 0);
        instance.BALL_GRAB_INVINCIBILITY_TIME = reader.GetInteger("ball", "grab_invincibility_time", 0);

        instance.SPIKE_RADIUS = reader.GetReal("spike", "radius", 0);

        instance.BOMB_RADIUS           = reader.GetReal("bomb", "radius", 0);
        instance.BOMB_RESPAWN_TIME     = reader.GetInteger("bomb", "respawn_time", 0);
        instance.BOMB_EXPLOSION_RADIUS = reader.GetReal("bomb", "explosion_radius", 0);
        instance.BOMB_EXPLOSION_FORCE  = reader.GetReal("bomb", "explosion_force", 0);

        instance.TOGGLE_RADIUS = reader.GetReal("toggle", "radius", 0);

        instance.BOOSTER_RADIUS       = reader.GetReal("booster", "radius", 0);
        instance.BOOSTER_RESPAWN_TIME = reader.GetInteger("booster", "respawn_time", 0);
        instance.BOOSTER_FORCE        = reader.GetReal("booster", "force", 0);
        instance.BOOSTER_MAX_V_TO_FIRE= reader.GetReal("booster", "max_v_to_fire", 0);

        instance.POWERUP_RADIUS       = reader.GetReal("powerup", "radius", 0);
        instance.POWERUP_RESPAWN_TIME = reader.GetInteger("powerup", "respawn_time", 0);
        instance.POWERUP_LASTING_TIME = reader.GetInteger("powerup", "lasting_time", 0);
        instance.ROLLING_BOMB_FORCE   = reader.GetReal("rolling_bomb", "exp_force", 0);
        instance.ROLLING_BOMB_RADIUS   = reader.GetReal("rolling_bomb", "exp_radius", 0);

        instance.FLAG_RADIUS = reader.GetReal("flag", "radius", 0);

        instance.PORTAL_RADIUS       = reader.GetReal("portal", "radius", 0);
        instance.PORTAL_RESPAWN_TIME = reader.GetInteger("portal", "respawn_time", 0);

        instance.COLOR_TILE = color(reader.Get("color", "tile", "000000"));
        instance.COLOR_WALL = color(reader.Get("color", "wall", "000000"));

        instance.GUI_INITIAL_WINDOW_WIDTH  = reader.GetInteger("gui", "initial_window_width", 0);
        instance.GUI_INITIAL_WINDOW_HEIGHT = reader.GetInteger("gui", "initial_window_height", 0);
        instance.GUI_MOVEMENT_SPEED_DIV    = reader.GetReal("gui", "movement_speed_div", 0);
        instance.GUI_TEXTURE               = reader.Get("gui", "texture", "");
        instance.GUI_ANTIALIASING          = reader.GetInteger("gui", "antialiasing", 0);
        instance.GUI_POLY_EXTRUDE          = reader.GetReal("gui", "poly_extrude", 0);


        instance.WORLD_FRAMERATE       = reader.GetInteger("world", "framerate", 0);
        instance.WORLD_VELO_ITERATIONS = reader.GetInteger("world", "velo_iterations", 0);
        instance.WORLD_POS_ITERATIONS  = reader.GetInteger("world", "pos_iterations", 0);

        instance.SERVER_LOBBY_PORT      = reader.GetReal("server", "lobby_port", 0);
        instance.SERVER_GAMES           = reader.GetReal("server", "games", 0);
        instance.SERVER_GAME_PORT_START = reader.GetReal("server", "game_port_start", 0);
        instance.SERVER_BALLSYNC_EVERY  = reader.GetInteger("server", "ballsync_every", 0);
        instance.SERVER_MAPS            = split_on(reader.Get("server", "maps", ""), '\n');
        for(auto && m : instance.SERVER_MAPS) {
            boost::algorithm::trim(m);
        }

        instance.GAME_MAX_LENGTH  = reader.GetInteger("game", "max_length", 0);
        instance.GAME_MAX_POINTS  = reader.GetInteger("game", "max_points", 0);
        instance.GAME_MAX_PLAYERS = reader.GetInteger("game", "max_players", 0);


        instance.is_initialized = true;
    }

    return instance;
}

