#include "ball.hpp"

thread_local std::size_t ball::id_counter = 0;

ball::ball(const ball_type type)
: id(id_counter++)
, type(type)
, body(nullptr)
, col_data(nullptr)
, portal_transport_ptr(nullptr)
, is_alive(true)
, player_ptr(nullptr)
, in_gate_ptrs({})
, on_tile_speed_counter(0)
, on_tile_endzone_counter(0)
, grab_invincibility_counter(0)
{}

ball::~ball()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
    }
}

void ball::add_to_world(b2World * world)
{
    const settings& config = settings::get_instance();

    b2BodyDef bdef;
    bdef.type = b2_dynamicBody;
    bdef.position.Set(10.0f, 10.0f);
    bdef.angle = 0.0f;

    body = world->CreateBody(&bdef);

    b2CircleShape bshape;
    bshape.m_p.Set(0.0f, 0.0f);
    bshape.m_radius = config.BALL_RADIUS;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.density = config.BALL_DENSITY;
    fdef.friction = config.BALL_FRICTION;
    fdef.restitution = config.BALL_RESTITUTION;
    body->CreateFixture(&fdef);
    body->SetLinearDamping(config.BALL_DAMPING);
    body->ResetMassData();
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(collision_user_data_type::ball, this));
    body->SetUserData(static_cast<void*>(col_data.get()));

    is_alive = true;
    in_gate_ptrs.clear();
    on_tile_speed_counter = 0;
    on_tile_endzone_counter = 0;
    grab_invincibility_counter = 0;
}

void ball::set_player_ptr(player* p)
{
    player_ptr = p;
}

void ball::set_portal_transport(portal* p)
{
    portal_transport_ptr = p;
    player_ptr->stats.portals_hit++;
}

void ball::set_position(const b2Vec2 pos)
{
    body->SetTransform(pos, body->GetAngle());
}

void ball::move(const int x, const int y)
{
    const settings& config = settings::get_instance();

    const bool has_jukejuice = has_powerup(powerup_type::jukejuice);
    const bool has_speed_tile = on_tile_speed_counter > 0 && flags.empty();

    float f = config.BALL_MOVEMENT_SPEED;
    if(has_jukejuice)  f = std::max(f, config.BALL_JUKEJUICE_SPEED);
    if(has_speed_tile) f = std::max(f, config.BALL_SPEED_TILE_SPEED);

    const float a = angle_from_input(x, y);

    body->ApplyForce(
        b2Vec2(std::cos(a) * f, std::sin(a) * f),
        body->GetWorldCenter(),
        true
    );
}

void ball::pop()
{
    const settings& config = settings::get_instance();

    explosion(config.BALL_POP_RADIUS, config.BALL_POP_FORCE)
        .explode(body->GetPosition(), body->GetWorld());

    is_alive = false;
    respawn_counter = config.BALL_RESPAWN_TIME;

    reset_flags();
    player_ptr->stats.popped++;
    player_ptr->g->add_server_event(server_event(server_event_ball_popped(this)));
}

void ball::get_boosted()
{
    const settings& config = settings::get_instance();

    const b2Vec2 v = body->GetLinearVelocity();
    const float a = std::atan2(v.y, v.x);

    body->ApplyLinearImpulse(
        b2Vec2(
            std::cos(a) * config.BOOSTER_FORCE,
            std::sin(a) * config.BOOSTER_FORCE
        ),
        body->GetWorldCenter(),
        true
    );
    player_ptr->stats.boosters_hit++;
}

void ball::add_powerup(const powerup_type type)
{
    spdlog::get("game")->debug("powerup received");

    const settings& config = settings::get_instance();

    for(auto && o : powerups) {
        if(o.type == type) {
            spdlog::get("game")->debug("increase time");
            o.counter = config.POWERUP_LASTING_TIME;
            return;
        }
    }

    spdlog::get("game")->debug("add new powerup for {0:d}", config.POWERUP_LASTING_TIME);
    powerups.emplace_back(type, config.POWERUP_LASTING_TIME);
    player_ptr->stats.powerups_picked_up++;
}

bool ball::has_powerup(const powerup_type type)
{
    for(auto && o : powerups) {
        if(o.type == type) {
            return true;
        }
    }

    return false;
}

void ball::remove_powerup(const powerup_type type)
{
    powerups.erase(
        std::remove_if(
            powerups.begin(),
            powerups.end(),
            [&type](ball_powerup bp) {
                return bp.type == type;
            }
        ),
        powerups.end()
    );
}

void ball::rb_explode()
{
    const settings& config = settings::get_instance();

    explosion(config.ROLLING_BOMB_RADIUS, config.ROLLING_BOMB_FORCE)
        .explode(body->GetPosition(), body->GetWorld());

    remove_powerup(powerup_type::rollingbomb);
    player_ptr->g->add_server_event(server_event(server_event_ball_rb_explode(this)));
}

bool ball::has_flag(const flag_type type)
{
    for(auto && o : flags) {
        if(o.f->type == type) {
            return true;
        }
    }

    return false;
}

void ball::add_flag(flag* f)
{
    const settings& config = settings::get_instance();

    flags.emplace_back(f);
    grab_invincibility_counter = config.BALL_GRAB_INVINCIBILITY_TIME;
    player_ptr->g->add_server_event(server_event(server_event_flag_grabbed(this, f)));
    player_ptr->stats.flag_grabs++;
}

void ball::reset_flags()
{
    for(auto && f : flags) {
        f.f->is_alive = true;
        player_ptr->stats.flag_drops++; // todo fixme : we count caps as drops as well here
    }

    flags.clear();
}

void ball::score()
{
    player_ptr->g->score(this);
    player_ptr->stats.flag_captures += flags.size();
    reset_flags();
}


void ball::take_flags(ball* o, const flag_type type)
{
    for(auto && f : o->flags) {
        if(f.f->type == type) {
            add_flag(f.f);
        }
    }

    // todo we need to add flag drops for these to o player
    // probably need a refactor
    o->flags.erase(
        std::remove_if(
            o->flags.begin(),
            o->flags.end(),
            [type](const ball_flag f) {
                return f.f->type == type;
            }
        ),
        o->flags.end()
    );
}
