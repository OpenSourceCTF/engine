#include "game.hpp"

game::game(){}

game::game(const std::uint16_t port, map* m)
: port(port)
, m(m)
, max_points(3)
, max_length(15*60)
, red_points(0)
, blue_points(0)
, world(nullptr)
, timestep(0)
{}

void game::spawn_srv_thread()
{
    srv_thread = std::thread(
        start_game_server,
        port
    );
    srv_thread.detach();
}

void game::spawn_phys_thread()
{
    phys_thread = std::thread(
        &game::run, std::ref(*this)
    );
    phys_thread.detach();
}

void game::run()
{
    const settings& config = settings::get_instance();
    world = init_world();

    // consider std::sleep_until here
    while(true) {
        if(players.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            continue;
        }

        const std::chrono::high_resolution_clock::time_point t_begin {
            std::chrono::high_resolution_clock::now()
        };

        if(timestep % config.SERVER_BALLSYNC_EVERY == 0) {
            add_server_event(server_event(server_event_ballsync(this)));
        }

        handle_server_events();

        this->step();
        world->Step(
            1.0f/config.WORLD_FRAMERATE,
            config.WORLD_VELO_ITERATIONS,
            config.WORLD_POS_ITERATIONS
        );

        const std::chrono::high_resolution_clock::time_point t_end {
            std::chrono::high_resolution_clock::now()
        };

        // how long it takes to run 1 game&physics step
        const auto step_duration {
            std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_begin)
        };

        // how long we have available
        const auto tic_duration {
            std::chrono::microseconds(1000000 / config.WORLD_FRAMERATE)
        };

        const std::chrono::microseconds t_sleep(tic_duration - step_duration);

        spdlog::get("game")->debug("{0:d}ms", std::chrono::duration_cast<std::chrono::milliseconds>(step_duration).count());

        std::this_thread::sleep_for(t_sleep);
    }
}

void game::handle_server_events()
{
    std::lock_guard<std::mutex> lock(server_events_queue_mutex);

    while(! server_events_queue.empty()) {
        const server_event a = std::move(server_events_queue.front());

        switch(a.type) {
        case server_event_type::player_joined: {
            server_event_player_joined* m = static_cast<server_event_player_joined*>(a.ptr);
            try_broadcast(this, game_event(game_event_player_joined(m->p)));
        } break;

        case server_event_type::player_left: {
            server_event_player_left* m = static_cast<server_event_player_left*>(a.ptr);
            try_broadcast(this, game_event(game_event_player_left(m->p)));
        } break;

        case server_event_type::chat: {
            server_event_chat* m = static_cast<server_event_chat*>(a.ptr);
            try_broadcast(this, game_event(game_event_chat(m->p, m->msg)));
        } break;

        case server_event_type::teamchat: {
            server_event_teamchat* m = static_cast<server_event_teamchat*>(a.ptr);
            try_broadcast_team(this, m->p->b->type, game_event(game_event_teamchat(m->p, m->msg)));
        } break;

        case server_event_type::movement: {
            server_event_movement* m = static_cast<server_event_movement*>(a.ptr);
            m->p->xdir = m->xdir;
            m->p->ydir = m->ydir;
        } break;

        case server_event_type::honk: {
            server_event_honk* m = static_cast<server_event_honk*>(a.ptr);
            try_broadcast(this, game_event(game_event_honk(m->p)));
        } break;

        case server_event_type::ballsync: {
            server_event_ballsync* m = static_cast<server_event_ballsync*>(a.ptr);
            try_broadcast(this, game_event(game_event_ballsync(m->g)));
        } break;

        case server_event_type::ball_respawn: {
            server_event_ball_respawn* m = static_cast<server_event_ball_respawn*>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_respawn(
                m->m->id,
                m->m->body->GetPosition()
            )));
        } break;

        case server_event_type::bomb_respawn: {
            server_event_bomb_respawn* m = static_cast<server_event_bomb_respawn*>(a.ptr);
            try_broadcast(this, game_event(game_event_bomb_respawn(
                m->m->id,
                m->m->body->GetPosition()
            )));
        } break;

        case server_event_type::bomb_explosion: {
            server_event_bomb_explosion* m = static_cast<server_event_bomb_explosion*>(a.ptr);
            try_broadcast(this, game_event(game_event_bomb_explosion(
                m->m->id
            )));
        } break;

        case server_event_type::powerup_respawn: {
            server_event_powerup_respawn* m = static_cast<server_event_powerup_respawn*>(a.ptr);
            try_broadcast(this, game_event(game_event_powerup_respawn(
                m->m->id,
                m->m->body->GetPosition(),
                m->m->type
            )));
        } break;

        case server_event_type::booster_respawn: {
            server_event_booster_respawn* m = static_cast<server_event_booster_respawn*>(a.ptr);
            try_broadcast(this, game_event(game_event_booster_respawn(
                m->m->id,
                m->m->body->GetPosition()
            )));
        } break;

        case server_event_type::portal_respawn: {
            server_event_portal_respawn* m = static_cast<server_event_portal_respawn*>(a.ptr);
            try_broadcast(this, game_event(game_event_portal_respawn(
                m->m->id,
                m->m->body->GetPosition()
            )));
        } break;

        case server_event_type::ball_popped: {
            server_event_ball_popped* m = static_cast<server_event_ball_popped*>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_popped(
                m->m->id,
                m->m->body->GetPosition()
            )));
        } break;

        case server_event_type::ball_boosted: {
            server_event_ball_boosted* m = static_cast<server_event_ball_boosted*>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_boosted(
                m->m_ball->id,
                m->m_booster->id
            )));
        } break;

        case server_event_type::ball_score: {
            server_event_ball_score* m = static_cast<server_event_ball_score*>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_score(
                m->m->id
            )));
        } break;

        case server_event_type::ball_portal: {
            server_event_ball_portal* m = static_cast<server_event_ball_portal*>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_portal(
                m->m_ball->id,
                m->m_portal->id
            )));
        } break;

        case server_event_type::ball_powerup: {
            server_event_ball_powerup* m = static_cast<server_event_ball_powerup*>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_powerup(
                m->m_ball->id,
                m->m_powerup->id
            )));
        } break;

        case server_event_type::flag_grabbed: {
            server_event_flag_grabbed* m = static_cast<server_event_flag_grabbed*>(a.ptr);
            try_broadcast(this, game_event(game_event_flag_grabbed(
                m->m_ball->id,
                m->m_flag->id
            )));
        } break;

        case server_event_type::toggle_on: {
            server_event_toggle_on* m = static_cast<server_event_toggle_on*>(a.ptr);
            try_broadcast(this, game_event(game_event_toggle_on(
                m->m_ball->id,
                m->m_toggle->id
            )));
        } break;

        case server_event_type::toggle_off: {
            server_event_toggle_off* m = static_cast<server_event_toggle_off*>(a.ptr);
            try_broadcast(this, game_event(game_event_toggle_off(
                m->m_ball->id,
                m->m_toggle->id
            )));
        } break;

        default:
            spdlog::get("game")->error("server_event_type ", to_string(a.type), " not enumerated in handle_server_events");
            break;
        }

        server_events_queue.pop();
    }

    // remove all players marked for removal
    players.erase(
        std::remove_if(
            players.begin(),
            players.end(),
            [](std::unique_ptr<player> & p) {
                return p->remove;
            }
        ),
        players.end()
    );
}

void game::change_map(map* m)
{
    std::map<player*, ball_type> player_balls;


    if(this->m) {
        for(auto && o : players) {
            player_balls[o.get()] = o->b->type;
        }

        delete this->m;
    }

    this->m = m;
    init_world();
    for(auto && o : player_balls) {
        player* p = o.first;
        ball_type t = o.second;
        ball* b = add_ball(new ball(t));
        p->b = b;
        b->set_player_ptr(p);
    }
}

void game::step()
{
    for(auto && o : players) {
        ball* b = o->b;
        if(b && b->is_alive) {
            if(o->xdir || o->ydir) {
                b->move(o->xdir, o->ydir);
            }
        }
    }

    for(auto && o : m->balls) {
        if(! o->is_alive && ! --(o->respawn_counter)) {
            respawn_ball(o.get());
            o->is_alive = true;
            add_server_event(server_event(server_event_ball_respawn(o.get())));
        }

        for(auto & p : o->powerups) {
            --p.counter;
        }

        o->powerups.erase(
            std::remove_if(
                o->powerups.begin(),
                o->powerups.end(),
                [](ball_powerup bp) {
                    return bp.counter == 0;
                }
            ),
            o->powerups.end()
        );

        if(o->portal_transport_ptr) {
            const portal* p = o->portal_transport_ptr;
            o->set_position(b2Vec2(p->x, p->y));
            o->portal_transport_ptr = nullptr;
        }
    }

    for(auto && o : m->bombs) {
        if(! o->is_alive && ! --o->respawn_counter) {
            o->is_alive = true;
            add_server_event(server_event(server_event_bomb_respawn(o.get())));
        }
    }

    for(auto && o : m->powerups) {
        if(! o->is_alive && ! --o->respawn_counter) {
            o->type = o->get_random_type();
            o->is_alive = true;
            add_server_event(server_event(server_event_powerup_respawn(o.get())));
        }
    }

    for(auto && o : m->boosters) {
        if(! o->is_alive && ! --o->respawn_counter) {
            o->is_alive = true;
            add_server_event(server_event(server_event_booster_respawn(o.get())));
        }
    }

    for(auto && o : m->portals) {
        if(! o->is_alive) {
            if(o->has_cooldown) {
                if(o->is_cooling_down && --(o->cooldown_counter) == 0) {
                    o->is_alive = true;
                    o->is_cooling_down = false;
                    add_server_event(server_event(server_event_portal_respawn(o.get())));
                }
            } else {
                o->is_alive = true;
                add_server_event(server_event(server_event_portal_respawn(o.get())));
            }
        }
    }

    ++timestep;
}

void game::respawn_ball(ball* b)
{
    spdlog::get("game")->debug("respawning");
    random_util& rng = random_util::get_instance();

    std::vector<spawn> potential_spawns;

    for(auto && s : m->spawns) {
        if(same_color(s->type, b->type)) {
            for(std::size_t i=0; i<(s->weight > 0) ? s->weight : 1; ++i) {
                potential_spawns.emplace_back(*s);
            }
        }
    }

    if(potential_spawns.empty()) {
        const spawn_type matching_spawn_type = corresponding_color<spawn_type>(b->type);

        // maps dont require spawn points...
        // but thats stupid
        // maybe this should be fixed in map export
        const flag_type matching_flag_type = corresponding_color<flag_type>(b->type);

        for(auto && f : m->flags) {
            if(f->type == matching_flag_type) {
                potential_spawns.emplace_back(f->x, f->y, 1, 1, matching_spawn_type);
            }
        }

        // if theres no flags or spawns...
        if(potential_spawns.empty()) {
            spdlog::get("game")->info("no spawns found, placing at 0,0, fix your map");
            potential_spawns.emplace_back(0, 0, 1, 1, matching_spawn_type);
        }
    }

    const spawn & s = potential_spawns[
        std::uniform_int_distribution<int>(0, potential_spawns.size()-1)(rng.eng)
    ];
    const float a = std::uniform_real_distribution<>(0.0f, TWO_PI)(rng.eng);
    b->set_position(b2Vec2(
        s.x + (std::cos(a) * s.radius),
        s.y + (std::sin(a) + s.radius))
    );
}

ball* game::add_ball(ball* b)
{
    m->balls.emplace_back(b);

    ball* B = m->balls.back().get();

    B->add_to_world(world);
    respawn_ball(B);

    return B;
}


// this maybe should be broken up
b2World * game::init_world()
{
    world = new b2World(b2Vec2(0, 0));

    // todo: does this need to be thread_local
    thread_local static contact_listener contact_listener_instance;
    world->SetContactListener(&contact_listener_instance);

    for(auto && o : m->spikes) {
        o->add_to_world(world);
        o->game.set_game(this);
    }

    for(auto && o : m->bombs) {
        o->add_to_world(world);
        o->game.set_game(this);
    }

    for(auto && o : m->toggles) {
        o->add_to_world(world);
        o->game.set_game(this);
    }

    for(auto && o : m->boosters) {
        o->add_to_world(world);
        o->game.set_game(this);
    }

    for(auto && o : m->powerups) {
        o->add_to_world(world);
        o->game.set_game(this);
    }

    for(auto && o : m->flags) {
        o->add_to_world(world);
        o->game.set_game(this);
    }

    for(auto && o : m->portals) {
        o->add_to_world(world);
        o->game.set_game(this);
    }

    for(auto && o : m->chains) {
        o->add_to_world(world);
    }

    return world;
}

player* game::add_player(player* p)
{
    players.emplace_back(p);
    return players.back().get();
}

void game::score(ball* b)
{
    spdlog::get("game")->debug("score!");

    if(b->type == ball_type::red) {
        ++red_points;
    }

    if(b->type == ball_type::blue) {
        ++blue_points;
    }

    add_server_event(server_event(server_event_ball_score(b)));
}

player* game::get_player_from_con(websocketpp::connection_hdl con)
{
    for(auto && o : players) {
        if(o->con.lock() == con.lock()) {
            return o.get();
        }
    }

    return nullptr;
}

void game::add_server_event(server_event a)
{
    std::lock_guard<std::mutex> lock(server_events_queue_mutex);
    server_events_queue.emplace(a);
}
