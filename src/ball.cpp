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
, jumps_since_land_counter(0)
, grab_invincibility_counter(0)
{}

ball::~ball()
{
    if(body) {
        body->GetWorld()->DestroyBody(body);
        body = reinterpret_cast<b2Body*>(0xDEADBEEF); // debug helper
    }
}

void ball::add_to_world(b2World * world)
{
    const settings& config = settings::get_instance();

    b2BodyDef bdef;
    bdef.type = b2_dynamicBody;
    bdef.position.Set(10.0f, 10.0f);
    bdef.angle = 0.0f;
    bdef.linearDamping = config.BALL_DAMPING;
    bdef.angularDamping = config.BALL_ANGULAR_DAMPING;

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
    jumps_since_land_counter = 0;
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

void ball::move(const int x, const int y)
{
    const settings& config = settings::get_instance();

    const bool is_jumping = player_ptr->g->m->jumping_enabled && y == -1;

    // check max speed
    const b2Vec2 v = body->GetLinearVelocity();
    const int X = ((v.x >  config.BALL_MAX_MOVEMENT_SPEED && x>0)
                || (v.x < -config.BALL_MAX_MOVEMENT_SPEED && x<0)) ? 0 : x;

    const int Y = ((v.y >  config.BALL_MAX_MOVEMENT_SPEED && y>0)
                || (v.y < -config.BALL_MAX_MOVEMENT_SPEED && y<0)) ? 0 : y;

    if(X == 0 && Y == 0) {
        return;
    }

    const float a = angle_from_input(X, Y);

    // alter speed modifiers
    const bool has_jukejuice = has_powerup(powerup_type::jukejuice);
    const bool has_speed_tile = on_tile_speed_counter > 0 && flags.empty();

    float s = config.BALL_MOVEMENT_SPEED;
    if(has_jukejuice)  s = std::max(s, config.BALL_JUKEJUICE_SPEED);
    if(has_speed_tile) s = std::max(s, config.BALL_SPEED_TILE_SPEED);

    auto jump = [&]() {
        body->ApplyLinearImpulse(
            b2Vec2(0, -config.BALL_JUMP_SPEED),
            body->GetWorldCenter(),
            true
        );
    };

    if(is_jumping) {
        if(jumps_since_land_counter < 1 && v.y > -config.BALL_JUMP_MAX_V) {
            jump();
            ++jumps_since_land_counter;
        } else if(jumps_since_land_counter < 2 && std::abs(v.y) < config.BALL_JUMP_MAX_V_2) {
            if(v.y < 0) jump();
            ++jumps_since_land_counter;
        }
    }

    body->ApplyForce(
        b2Vec2(std::cos(a) * s, player_ptr->g->m->jumping_enabled ? 0 : std::sin(a) * s),
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

    b2Vec2 dir = body->GetLinearVelocity();
    const float speed = dir.Normalize();

    if(speed <= config.BOOSTER_MAX_V_TO_FIRE) {
        body->ApplyLinearImpulse(
            b2Vec2(
                dir.x * config.BOOSTER_FORCE,
                dir.y * config.BOOSTER_FORCE
            ),
            body->GetWorldCenter(),
            true
        );
    }

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
    flag* tflag = nullptr;
    for(auto && f : o->flags) {
        if(f.f->type == type) {
            add_flag(f.f);
            tflag = f.f;
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

    if(tflag) {
        player_ptr->g->add_server_event(server_event(server_event_flag_transferred(o, this, tflag)));
    }
}
