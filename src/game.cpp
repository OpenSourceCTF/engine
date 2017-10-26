#include "game.hpp"

game::game(const std::uint16_t port, map* m)
: port(port)
, m(m)
, max_points(3)
, max_length(15*60)
, world(nullptr)
{}

std::thread game::spawn_thread()
{
    std::thread thread(
        &game::run, *this
    );
    thread.detach();

    return thread;
}

void game::run()
{
    world = init_world();

    while(true) {
        this->step();
        world->Step(1/60.0, 8, 3);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
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
                std::cout << o->cooldown_counter << std::endl;
                if(o->is_cooling_down && --(o->cooldown_counter) == 0) {
                    o->is_alive = true;
                    o->is_cooling_down = false;
                }
            } else {
                o->is_alive = true;
            }
        }
    }
}

void game::respawn_ball(ball* b)
{
    std::cout << "respawning" << std::endl;
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
            std::cerr << "error: no spawns found, placing at 0,0, fix your map" << std::endl;
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

ball* game::add_ball(b2World* world, ball b)
{
    m->balls.emplace_back(new ball(b));
    m->balls.back()->add_to_world(world);
    respawn_ball(m->balls.back().get());

    return m->balls.back().get();
}


b2World * game::init_world()
{
    b2World* world = new b2World(b2Vec2(0, 0));
    // todo: does this need to be thread_local
    thread_local static contact_listener contact_listener_instance;
    world->SetContactListener(&contact_listener_instance);

    for(std::size_t i=0; i<8; ++i) {
        add_ball(world, ball(i < 4 ? ball_type::red : ball_type::blue));
    }

    for(auto && o : m->walls) {
        o->add_to_world(world);
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

    return world;
}

void to_json(nlohmann::json& j, const game& p)
{
    // todo
    // we want score and time remaining here as well
    j = nlohmann::json{
        {"port",         p.port},
        {"max_points",   p.max_points},
        {"max_length",   p.max_length},
        {"map_name",     p.m->name},
        {"map_author",   p.m->author},
        {"player_count", p.m->balls.size()}
    };
}
