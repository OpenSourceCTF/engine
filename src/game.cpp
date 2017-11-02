#include "game.hpp"

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

std::thread game::spawn_thread()
{
    std::thread thread(
        &game::run, std::ref(*this)
    );
    thread.detach();

    return thread;
}

void game::run()
{
    const settings& config = settings::get_instance();
    world = init_world();

    // consider std::sleep_until here
    while(true) {
        const std::chrono::high_resolution_clock::time_point t_begin {
            std::chrono::high_resolution_clock::now()
        };

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

        // spdlog::get("game")->debug("{0:d}ms", std::chrono::duration_cast<std::chrono::milliseconds>(step_duration).count());

        std::this_thread::sleep_for(t_sleep);
    }
}

void game::handle_client_actions()
{
    while(! client_actions_queue.empty()) {
        std::lock_guard<std::mutex> lock(client_actions_queue_mutex);
        const client_action a = std::move(client_actions_queue.front());

        // todo: handle all actions
        switch(a.type) {
        case client_action_type::player_joined: {
            client_action_player_joined* m = static_cast<client_action_player_joined*>(a.ptr);
            try_broadcast(this, game_event(game_event_player_joined(m->p)));
        } break;

        case client_action_type::chat: {
            client_action_chat* m = static_cast<client_action_chat*>(a.ptr);
            try_broadcast(this, game_event(game_event_chat(m->p, m->msg)));
        } break;

        default:
            spdlog::get("game")->error("client_action_type ", to_string(a.type), " not enumerated in handle_client_actions");
            break;
        }

        client_actions_queue.pop();
    }
}



void game::step()
{
    for(auto && o : m->balls) {
        if(! o->is_alive && ! --(o->respawn_counter)) {
            respawn_ball(o.get());
            o->is_alive = true;
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
        }
    }

    for(auto && o : m->powerups) {
        if(! o->is_alive && ! --o->respawn_counter) {
            o->type = o->get_random_type();
            o->is_alive = true;
        }
    }

    for(auto && o : m->boosters) {
        if(! o->is_alive && ! --o->respawn_counter) {
            o->is_alive = true;
        }
    }

    for(auto && o : m->portals) {
        if(! o->is_alive) {
            if(o->has_cooldown) {
                if(o->is_cooling_down && --(o->cooldown_counter) == 0) {
                    o->is_alive = true;
                    o->is_cooling_down = false;
                }
            } else {
                o->is_alive = true;
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

ball* game::add_ball(ball b)
{
    m->balls.emplace_back(new ball(b));

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

    for(std::size_t i=0; i<4; ++i) {
        ball* b = add_ball(ball(i % 2 ? ball_type::red : ball_type::blue));
        player* p = add_player(player(this, b, "player_id", true, "name", 100));
        b->set_player_ptr(p);
    }

    for(auto && o : m->spikes) {
        o->add_to_world(world);
    }

    for(auto && o : m->bombs) {
        o->add_to_world(world);
    }

    for(auto && o : m->toggles) {
        o->add_to_world(world);
    }

    for(auto && o : m->boosters) {
        o->add_to_world(world);
    }

    for(auto && o : m->powerups) {
        o->add_to_world(world);
    }

    for(auto && o : m->flags) {
        o->add_to_world(world);
    }

    for(auto && o : m->portals) {
        o->add_to_world(world);
    }

    for(auto && o : m->chains) {
        o->add_to_world(world);
    }

    return world;
}

player* game::add_player(player p)
{
    players.emplace_back(new player(p));
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

void game::add_client_action(client_action a)
{
    // todo use locks and crap
    client_actions_queue.emplace(a);
}
