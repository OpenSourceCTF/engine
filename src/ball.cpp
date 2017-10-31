#include "ball.hpp"

ball::ball(const ball_type type)
: type(type)
, body(nullptr)
, col_data(nullptr)
, portal_transport_ptr(nullptr)
, is_alive(true)
{
    const settings& config = settings::get_instance();
    pop_ex = explosion(config.BALL_POP_RADIUS,
                       config.BALL_POP_FORCE);
    rb_ex = explosion(config.ROLLING_BOMB_RADIUS,
                      config.ROLLING_BOMB_FORCE);
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
    col_data = std::shared_ptr<collision_user_data>(new collision_user_data(this));
    body->SetUserData(static_cast<void*>(col_data.get()));

    is_alive = true;
}

void ball::set_portal_transport(portal* p)
{
    portal_transport_ptr = p;
}

void ball::set_position(const b2Vec2 pos)
{
    body->SetTransform(pos, get_angle());
}

void ball::move(const int x, const int y)
{
    const settings& config = settings::get_instance();

    const float f = (has_powerup(powerup_type::jukejuice))
        ? config.BALL_JUKEJUICE_SPEED
        : config.BALL_MOVEMENT_SPEED;

    const float a = angle_from_input(x, y);

    body->ApplyForce(
        b2Vec2(std::cos(a) * f, std::sin(a) * f),
        body->GetWorldCenter(),
        true
    );
}

b2Vec2 ball::get_position() const
{
    return body->GetPosition();
}

float ball::get_angle() const
{
    return body->GetAngle();
}

b2Vec2 ball::get_linear_velocity() const
{
    return body->GetLinearVelocity();
}

void ball::pop()
{
    const settings& config = settings::get_instance();

    auto pos = this->get_position();
    pop_ex.explode(pos.x,pos.y,body->GetWorld());

    is_alive = false;
    respawn_counter = config.BOOSTER_RESPAWN_TIME;

    reset_flags();
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
}

void ball::add_powerup(const powerup_type type)
{
    std::cout << "powerup received" << std::endl;
    std::cout << get_position().x << std::endl;
    const settings& config = settings::get_instance();

    for(auto & o : powerups) {
        if(o.type == type) {
            std::cout << "increase time" << std::endl;
            o.counter = config.POWERUP_LASTING_TIME;
            return;
        }
    }

    std::cout << "add new powerup for:" << config.POWERUP_LASTING_TIME << std::endl;
    powerups.emplace_back(type, config.POWERUP_LASTING_TIME);
}

bool ball::has_powerup(const powerup_type type)
{
    for(auto & o : powerups) {
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
    // todo explosion here
    std::cout << "rollingboom" << std::endl;
    auto pos = this->get_position();
    rb_ex.explode(pos.x,pos.y,body->GetWorld());
    remove_powerup(powerup_type::rollingbomb);
}

bool ball::has_flag(const flag_type type)
{
    for(auto & o : flags) {
        if(o.f->type == type) {
            return true;
        }
    }

    return false;
}

void ball::add_flag(flag* f)
{
    std::cout << "add new flag" << std::endl;
    flags.emplace_back(f);
}

void ball::reset_flags()
{
    for(auto & f : flags) {
        f.f->is_alive = true;
    }

    flags.clear();
}

void ball::score()
{
    std::cout << "score, yay" << std::endl;
    reset_flags();
}
