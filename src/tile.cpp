#include "tile.hpp"
#include <json/json.hpp>

tile::tile(){}
tile::tile(
    const polygon poly,
    const tile_type type
)
: poly(poly)
, type(type)
, body(nullptr)
, col_data(nullptr)
{}

void tile::add_to_world(b2World * world)
{
	// we dont need world items for these things
	switch(type) {
		case tile_type::normal:
		case tile_type::background:
			return;
		default: break;
	}

    const b2Vec2 center = poly.get_center();

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(center.x, center.y);
    bdef.angle = 0.0f;

    body = world->CreateBody(&bdef);

    const std::array<b2Vec2, 3> vertices = poly.get_vertices();

    b2PolygonShape bshape;
    bshape.Set(vertices.data(), vertices.size());

    b2FixtureDef fdef;
    fdef.shape = &bshape;
    fdef.isSensor = true;
    body->CreateFixture(&fdef);

    {
        collision_user_data_type c_type;
        switch(type) {
            case tile_type::speed_red:
                c_type = collision_user_data_type::speed_red;
                break;
            case tile_type::speed_blue:
                c_type = collision_user_data_type::speed_blue;
                break;
            case tile_type::endzone_red:
                c_type = collision_user_data_type::endzone_red;
                break;
            case tile_type::endzone_blue:
                c_type = collision_user_data_type::endzone_blue;
                break;
            default: // this should never actually be called
                spdlog::get("game")->critical("c_type default called");
                c_type = collision_user_data_type::unknown;
                break;
        }

        col_data = std::shared_ptr<collision_user_data>(new collision_user_data(c_type, this));
    }

    body->SetUserData(static_cast<void*>(col_data.get()));
}

void to_json(nlohmann::json& j, const tile& p)
{
    j = nlohmann::json{{"poly", p.poly}, {"type", to_string(p.type)}};
}

void from_json(const nlohmann::json& j, tile& p)
{
    p = tile(
        j.at("poly").get<polygon>(),
        tile_type_from_string(j.at("type").get<std::string>())
    );
}
