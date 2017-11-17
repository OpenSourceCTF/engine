#include "game.hpp"
#include <json/json.hpp>

game::game(const std::uint16_t port)
: port(port)
, m(nullptr)
, red_points(0)
, blue_points(0)
, world(nullptr)
, timestep(0)
, finished(false)
{}

bool game::spawn_srv_thread()
{
    if(! m) {
        spdlog::get("game")->critical("map not loaded, cannot start srv thread");
        return false;
    }

    srv_thread = std::thread(
        start_game_server,
        port
    );
    srv_thread.detach();
    return true;
}

bool game::spawn_phys_thread()
{
    if(! m) {
        spdlog::get("game")->critical("map not loaded, cannot start phys thread");
        return false;
    }

    phys_thread = std::thread(
        &game::run, std::ref(*this)
    );
    phys_thread.detach();
    return true;
}

void game::run()
{
    const settings& config = settings::get_instance();

    // consider std::sleep_until here
    while(true) {
        if(players.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            continue;
        }

        const std::chrono::high_resolution_clock::time_point t_begin {
            std::chrono::high_resolution_clock::now()
        };

        handle_server_events();
        if(! finished) {
            this->step();
            if(timestep % config.SERVER_BALLSYNC_EVERY == 0) {
                add_server_event(server_event(server_event_ballsync(this)));
            }
        }

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

        // only allow these commands to go through, drop everything else
        if(finished) switch(a.type) {
            case server_event_type::player_joined:
            case server_event_type::player_left:
            case server_event_type::chat:
            case server_event_type::teamchat:
            case server_event_type::honk:
            case server_event_type::game_finished:
                break;

            default: goto pop_server_event;
        }

        switch(a.type) {
        case server_event_type::gamesync: {
            auto m = std::static_pointer_cast<server_event_gamesync>(a.ptr);
            try_send(m->p->srv, m->p->con, websocketpp::frame::opcode::value::text, 
                game_event(game_event_gamesync(this))
            );
        } break;

        case server_event_type::player_joined: {
            auto m = std::static_pointer_cast<server_event_player_joined>(a.ptr);
            try_broadcast(this, game_event(game_event_player_joined(m->p)));
        } break;

        case server_event_type::player_left: {
            auto m = std::static_pointer_cast<server_event_player_left>(a.ptr);
            try_broadcast(this, game_event(game_event_player_left(m->p)));
        } break;

        case server_event_type::chat: {
            auto m = std::static_pointer_cast<server_event_chat>(a.ptr);
            try_broadcast(this, game_event(game_event_chat(m->p, m->msg)));
        } break;

        case server_event_type::teamchat: {
            auto m = std::static_pointer_cast<server_event_teamchat>(a.ptr);
            try_broadcast_team(this, m->p->b->type, game_event(game_event_teamchat(m->p, m->msg)));
        } break;

        case server_event_type::movement: {
            auto m = std::static_pointer_cast<server_event_movement>(a.ptr);
            m->p->xdir = m->xdir;
            m->p->ydir = m->ydir;
        } break;

        case server_event_type::honk: {
            auto m = std::static_pointer_cast<server_event_honk>(a.ptr);
            try_broadcast(this, game_event(game_event_honk(m->p)));
        } break;

        case server_event_type::ballsync: {
            auto m = std::static_pointer_cast<server_event_ballsync>(a.ptr);
            try_broadcast(this, game_event(game_event_ballsync(m->g)));
        } break;

        case server_event_type::ball_respawn: {
            auto m = std::static_pointer_cast<server_event_ball_respawn>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_respawn(
                m->m->id,
                m->m->body->GetPosition()
            )));
        } break;

        case server_event_type::bomb_respawn: {
            auto m = std::static_pointer_cast<server_event_bomb_respawn>(a.ptr);
            try_broadcast(this, game_event(game_event_bomb_respawn(
                m->m->id,
                m->m->body->GetPosition()
            )));
        } break;

        case server_event_type::bomb_explosion: {
            auto m = std::static_pointer_cast<server_event_bomb_explosion>(a.ptr);
            try_broadcast(this, game_event(game_event_bomb_explosion(
                m->m->id
            )));
        } break;

        case server_event_type::powerup_respawn: {
            auto m = std::static_pointer_cast<server_event_powerup_respawn>(a.ptr);
            try_broadcast(this, game_event(game_event_powerup_respawn(
                m->m->id,
                m->m->body->GetPosition(),
                m->m->type
            )));
        } break;

        case server_event_type::booster_respawn: {
            auto m = std::static_pointer_cast<server_event_booster_respawn>(a.ptr);
            try_broadcast(this, game_event(game_event_booster_respawn(
                m->m->id,
                m->m->body->GetPosition()
            )));
        } break;

        case server_event_type::portal_respawn: {
            auto m = std::static_pointer_cast<server_event_portal_respawn>(a.ptr);
            try_broadcast(this, game_event(game_event_portal_respawn(
                m->m->id,
                m->m->body->GetPosition()
            )));
        } break;

        case server_event_type::ball_popped: {
            auto m = std::static_pointer_cast<server_event_ball_popped>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_popped(
                m->m->id,
                m->m->body->GetPosition()
            )));
        } break;

        case server_event_type::ball_boosted: {
            auto m = std::static_pointer_cast<server_event_ball_boosted>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_boosted(
                m->m_ball->id,
                m->m_booster->id
            )));
        } break;

        case server_event_type::ball_score: {
            auto m = std::static_pointer_cast<server_event_ball_score>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_score(
                m->m->id
            )));
        } break;

        case server_event_type::ball_portal: {
            auto m = std::static_pointer_cast<server_event_ball_portal>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_portal(
                m->m_ball->id,
                m->m_portal->id
            )));
        } break;

        case server_event_type::ball_powerup: {
            auto m = std::static_pointer_cast<server_event_ball_powerup>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_powerup(
                m->m_ball->id,
                m->m_powerup->id
            )));
        } break;

        case server_event_type::ball_rb_explode: {
            auto m = std::static_pointer_cast<server_event_ball_rb_explode>(a.ptr);
            try_broadcast(this, game_event(game_event_ball_rb_explode(
                m->m->id
            )));
        } break;

        case server_event_type::flag_grabbed: {
            auto m = std::static_pointer_cast<server_event_flag_grabbed>(a.ptr);
            try_broadcast(this, game_event(game_event_flag_grabbed(
                m->m_ball->id,
                m->m_flag->id
            )));
        } break;

        case server_event_type::toggle_on: {
            auto m = std::static_pointer_cast<server_event_toggle_on>(a.ptr);
            try_broadcast(this, game_event(game_event_toggle_on(
                m->m_ball->id,
                m->m_toggle->id
            )));
        } break;

        case server_event_type::toggle_off: {
            auto m = std::static_pointer_cast<server_event_toggle_off>(a.ptr);
            try_broadcast(this, game_event(game_event_toggle_off(
                m->m_ball->id,
                m->m_toggle->id
            )));
        } break;

        case server_event_type::game_finished: {
            auto m = std::static_pointer_cast<server_event_game_finished>(a.ptr);
            try_broadcast(this, game_event(game_event_game_finished()));
        } break;

        case server_event_type::game_stats: {
            auto m = std::static_pointer_cast<server_event_game_stats>(a.ptr);

            try_send(m->p->srv, m->p->con, websocketpp::frame::opcode::value::text, 
                game_event(game_event_game_stats(this))
            );
        } break;

        case server_event_type::vote_player: {
            auto m = std::static_pointer_cast<server_event_vote_player>(a.ptr);
            try_broadcast(this, game_event(game_event_vote_player(
                m->p,
                m->vp,
                m->reason
            )));
        } break;

        }

pop_server_event:
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

bool game::load_map(const std::string map_src)
{
    // reset these otherwise we'll keep increasing ids for next load
    gate::id_counter    = 0;
    booster::id_counter = 0;
    portal::id_counter  = 0;
    bomb::id_counter    = 0;
    ball::id_counter    = 0;
    powerup::id_counter = 0;
    flag::id_counter    = 0;
    toggle::id_counter  = 0;


    // cache player balls so we can add them again after loading map
    std::map<player*, ball_type> player_balls;
    for(auto && o : players) {
        player_balls[o.get()] = o->b->type;
    }


    // attempt to load new map
    try {
        std::ifstream t(map_src);
        std::stringstream buf;
        buf << t.rdbuf();
        nlohmann::json map_j = nlohmann::json::parse(buf.str());

        // clear old map
        if(m) {
            delete m;
        }

        m = new map(map_j);
        m->game.set_game(this);
    } catch(nlohmann::detail::parse_error e) {
        spdlog::get("game")->error("loading map failed:", e.what());
        return false;
    }

    // reset world
    // note: this must be called only after deleting map
    if(world) {
        delete world;
    }

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

    for(auto && o : m->tiles) {
        o->add_to_world(world);
        o->game.set_game(this);
    }

    for(auto && o : m->gates) {
        o->add_to_world(world);
    }

    // use our cache we made above to add balls to existing players
    for(auto && o : player_balls) {
        player* p = o.first;
        ball_type t = o.second;
        ball* b = add_ball(new ball(t));
        p->b = b;
        b->set_player_ptr(p);
    }

    return true;
}

void game::step()
{
    const settings& config = settings::get_instance();

    if(timestep    >= config.GAME_MAX_LENGTH
    || red_points  >= config.GAME_MAX_POINTS
    || blue_points >= config.GAME_MAX_POINTS) {
        add_server_event(server_event(server_event_game_finished(this)));
        finished = true;
        return;
    }

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

        for(auto && p : o->powerups) {
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

        if(! o->is_alive) {
            continue;
        }

        if(o->grab_invincibility_counter > 0) {
            o->grab_invincibility_counter--;
        }

        if(o->portal_transport_ptr) {
            const portal* p = o->portal_transport_ptr;
            o->set_position(b2Vec2(p->x, p->y));
            o->portal_transport_ptr = nullptr;
        }

        if(o->in_gate_ptrs.size()) {
            for(const auto& g : o->in_gate_ptrs) {
                g->kill_if_other(o);
            }
            if(!o->is_alive) o->in_gate_ptrs = {};
        }

        // check if we can score yet
        if(! o->flags.empty() && o->on_tile_endzone_counter > 0) {
            flag* score_flag = nullptr;

            for(auto && f : m->flags) {
                if(! f->is_alive && f->type == inv_corresponding_color<flag_type>(o->type)) {
                    score_flag = f.get();
                    break;
                }
            }

            if(score_flag) {
                o->score();
            }
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

    world->Step(
        1.0f/config.WORLD_FRAMERATE,
        config.WORLD_VELO_ITERATIONS,
        config.WORLD_POS_ITERATIONS
    );

    ++timestep;
}

// this is separated out because it is much more complex than the other respawns
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

player* game::get_player_from_player_id(const std::string & player_id)
{
    for(auto && o : players) {
        if(o->player_id == player_id) {
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
