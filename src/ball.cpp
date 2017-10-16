#include "ball.hpp"

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
    // memory leak
    body->SetUserData(static_cast<void*>(new collision_user_data(static_cast<void*>(this), collision_user_data_type::ball)));
}

void ball::move(const int x, const int y)
{
    const settings& config = settings::get_instance();

    const float a = angle_from_input(x, y);
    body->ApplyForce(
        b2Vec2(
            std::cos(a)*config.BALL_MOVEMENT_SPEED,
            std::sin(a)*config.BALL_MOVEMENT_SPEED
        ),
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
    std::cout << "popped" << std::endl;
}

void ball::get_boosted()
{
    std::cout << "boosted" << std::endl;
}

