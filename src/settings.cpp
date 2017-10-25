#include "settings.hpp"

settings& settings::get_instance()
{
    static settings instance;

    if(instance.is_initialized) {
        return instance;
    }

    INIReader reader("config.ini");
    if(reader.ParseError() < 0) {
        std::cerr << "error: cannot load 'config.ini'" << std::endl;
        return instance;
    } else {
        instance.BALL_MOVEMENT_SPEED  = reader.GetReal("ball", "movement_speed", 0);
        instance.BALL_JUKEJUICE_SPEED = reader.GetReal("ball", "jukejuice_speed", 0);
        instance.BALL_DAMPING         = reader.GetReal("ball", "damping", 0);
        instance.BALL_DENSITY         = reader.GetReal("ball", "density", 0);
        instance.BALL_FRICTION        = reader.GetReal("ball", "friction", 0);
        instance.BALL_RESTITUTION     = reader.GetReal("ball", "restitution", 0);
        instance.BALL_RADIUS          = reader.GetReal("ball", "radius", 0);
        instance.BALL_RESPAWN_TIME    = reader.GetInteger("ball", "respawn_time", 0);

        instance.SPIKE_RADIUS = reader.GetReal("spike", "radius", 0);

        instance.BOMB_RADIUS           = reader.GetReal("bomb", "radius", 0);
        instance.BOMB_RESPAWN_TIME     = reader.GetInteger("bomb", "respawn_time", 0);
        instance.BOMB_EXPLOSION_RADIUS = reader.GetReal("bomb", "explosion_radius", 0);
        instance.BOMB_EXPLOSION_FORCE  = reader.GetReal("bomb", "explosion_force", 0);

        instance.TOGGLE_RADIUS = reader.GetReal("toggle", "radius", 0);

        instance.BOOSTER_RADIUS       = reader.GetReal("booster", "radius", 0);
        instance.BOOSTER_RESPAWN_TIME = reader.GetInteger("booster", "respawn_time", 0);
        instance.BOOSTER_FORCE        = reader.GetReal("booster", "force", 0);

        instance.POWERUP_RADIUS       = reader.GetReal("powerup", "radius", 0);
        instance.POWERUP_RESPAWN_TIME = reader.GetInteger("powerup", "respawn_time", 0);
        instance.POWERUP_LASTING_TIME = reader.GetInteger("powerup", "lasting_time", 0);

        instance.FLAG_RADIUS = reader.GetReal("flag", "radius", 0);

        instance.PORTAL_RADIUS = reader.GetReal("portal", "radius", 0);

        instance.COLOR_TILE = color(reader.Get("color", "tile", "000000"));
        instance.COLOR_WALL = color(reader.Get("color", "wall", "000000"));

        instance.GUI_INITIAL_WINDOW_WIDTH  = reader.GetInteger("gui", "initial_window_width", 0);
        instance.GUI_INITIAL_WINDOW_HEIGHT = reader.GetInteger("gui", "initial_window_height", 0);
        instance.GUI_MOVEMENT_SPEED_DIV    = reader.GetReal("gui", "movement_speed_div", 0);

        instance.SERVER_LOBBY_PORT      = reader.GetReal("server", "lobby_port", 0);
        instance.SERVER_GAMES           = reader.GetReal("server", "games", 0);
        instance.SERVER_GAME_PORT_START = reader.GetReal("server", "game_port_start", 0);
        instance.SERVER_MAPS            = split_on(reader.Get("server", "maps", ""), '\n');
        for(auto & m : instance.SERVER_MAPS) {
            boost::algorithm::trim(m);
        }


        instance.is_initialized = true;
    }

    return instance;
}

