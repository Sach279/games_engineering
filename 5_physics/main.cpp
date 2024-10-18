#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <vector>

b2World* world;
std::vector<b2Body*> bodies;
std::vector<sf::RectangleShape*> sprites;

// 1 sfml unit = 30 physics units
const float physics_scale = 30.0f;
// inverse of physics_scale, useful for calculations
const float physics_scale_inv = 1.0f / physics_scale;
// Magic numbers for accuracy of physics simulation
const int32 velocityIterations = 6;
const int32 positionIterations = 2;

// Convert from b2Vec2 to a Vector2f
inline const sf::Vector2f bv2_to_sv2(const b2Vec2& in) {
    return sf::Vector2f(in.x * physics_scale, in.y * physics_scale);
}

// Convert from Vector2f to a b2Vec2
inline const b2Vec2 sv2_to_bv2(const sf::Vector2f& in) {
    return b2Vec2(in.x * physics_scale_inv, in.y * physics_scale_inv);
}

// Convert from screenspace.y to physics.y (as they are the other way around)
inline const sf::Vector2f invert_height(const sf::Vector2f& in, float gameHeight) {
    return sf::Vector2f(in.x, gameHeight - in.y);
}

// Create a Box2D body with a box fixture
b2Body* CreatePhysicsBox(b2World& World, const bool dynamic, const sf::Vector2f& position, const sf::Vector2f& size) {
    b2BodyDef BodyDef;
    BodyDef.type = dynamic ? b2_dynamicBody : b2_staticBody;
    BodyDef.position = sv2_to_bv2(position);
    b2Body* body = World.CreateBody(&BodyDef);

    b2PolygonShape Shape;
    Shape.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
    b2FixtureDef FixtureDef;
    FixtureDef.density = dynamic ? 10.f : 0.f;
    FixtureDef.friction = dynamic ? 0.8f : 1.f;
    FixtureDef.restitution = 1.0;
    FixtureDef.shape = &Shape;
    body->CreateFixture(&FixtureDef);
    return body;
}

// Create a Box2d body with a box fixture, from a sf::RectangleShape
b2Body* CreatePhysicsBox(b2World& world, const bool dynamic, const sf::RectangleShape& rs) {
    return CreatePhysicsBox(world, dynamic, rs.getPosition(), rs.getSize());
}

void init(float gameWidth, float gameHeight) {
    const b2Vec2 gravity(0.0f, -10.0f);
    world = new b2World(gravity);

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Wall Dimensions
    sf::Vector2f walls[] = {
        // Top
        sf::Vector2f(gameWidth * .5f, 5.f), sf::Vector2f(gameWidth, 10.f),
        // Bottom
        sf::Vector2f(gameWidth * .5f, gameHeight - 5.f), sf::Vector2f(gameWidth, 10.f),
        // Left
        sf::Vector2f(5.f, gameHeight * .5f), sf::Vector2f(10.f, gameHeight),
        // Right
        sf::Vector2f(gameWidth - 5.f, gameHeight * .5f), sf::Vector2f(10.f, gameHeight)
    };

    // Build Walls
    for (int i = 0; i < 8; i += 2) {
        auto s = new sf::RectangleShape();
        s->setPosition(walls[i]);
        s->setSize(walls[i + 1]);
        s->setOrigin(walls[i + 1] * 0.5f);
        s->setFillColor(sf::Color::Blue);
        sprites.push_back(s);

        auto b = CreatePhysicsBox(*world, false, walls[i], walls[i + 1]);
        bodies.push_back(b);
    }

    // Create Boxes
    for (int i = 1; i < 11; ++i) {
        auto s = new sf::RectangleShape();
        s->setPosition(sf::Vector2f(i * (gameWidth / 12.f), gameHeight * .7f));
        s->setSize(sf::Vector2f(50.0f, 50.0f));
        s->setOrigin(sf::Vector2f(25.0f, 25.0f));
        s->setFillColor(sf::Color::White);
        sprites.push_back(s);

        auto b = CreatePhysicsBox(*world, true, *s);
        b->ApplyAngularImpulse(5.0f, true);
        bodies.push_back(b);
    }
}

void Update(float gameHeight) {
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();

    // Step Physics world by dt (non-fixed timestep)
    world->Step(dt, velocityIterations, positionIterations);

    for (int i = 0; i < bodies.size(); ++i) {
        // Sync Sprites to physics position
        sprites[i]->setPosition(invert_height(bv2_to_sv2(bodies[i]->GetPosition()), gameHeight));
        // Sync Sprites to physics Rotation
        sprites[i]->setRotation((180 / b2_pi) * bodies[i]->GetAngle());
    }
}

int main() {
    const float gameWidth = 800.0f;
    const float gameHeight = 600.0f;

    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight), "Physics Simulation");

    init(gameWidth, gameHeight);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        Update(gameHeight);

        window.clear();
        for (auto& sprite : sprites) {
            window.draw(*sprite);
        }
        window.display();
    }

    for (auto s : sprites) {
        delete s;
    }
    delete world;
    return 0;
}
