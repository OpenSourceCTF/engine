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
    bshape.m_radius = 0.5;

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.density = config.BALL_DENSITY;
    fdef.friction = config.BALL_FRICTION;
    fdef.restitution = config.BALL_RESTITUTION;
    body->CreateFixture(&fdef);
    body->SetLinearDamping(config.BALL_DAMPING);
    body->ResetMassData();
}

void ball::move(int x, int y)
{
    const settings& config = settings::get_instance();

    body->ApplyForce(
        b2Vec2(x*config.BALL_MOVEMENT_SPEED, y*config.BALL_MOVEMENT_SPEED),
        body->GetWorldCenter(),
        true
    );
}

b2Vec2 ball::get_position()
{
    return body->GetPosition();
}

float ball::get_angle()
{
    return body->GetAngle();
}


b2Vec2 ball::get_linear_velocity()
{
    return body->GetLinearVelocity();
}

