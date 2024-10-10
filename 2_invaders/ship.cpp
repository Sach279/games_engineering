#include "ship.h"
#include "game.h"
using namespace sf;
using namespace std;

bool Invader::direction;
float Invader::speed;

Ship::Ship() {}

Ship::Ship(IntRect ir, ShipType shipType) : Sprite(), type(shipType) {
    _sprite = ir;
    setTexture(spritesheet);
    setTextureRect(_sprite);
}

// Provide the implementation for the Update method
void Ship::Update(const float& dt) {
    // Base implementation can be empty or contain common logic
}

// Provide implementation for the pure virtual destructor
Ship::~Ship() = default;

Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir, ShipType::INVADER) {
    setOrigin(Vector2f(16.f, 16.f));
    setPosition(pos);
}

void Invader::Update(const float& dt) {
    Ship::Update(dt);
    move(Vector2f(dt * (direction ? 1.0f : -1.0f) * speed, 0.0f));

    if ((direction && getPosition().x > gameWidth - 16) ||
        (!direction && getPosition().x < 16)) {
        direction = !direction;
        for (int i = 0; i < ships.size(); ++i) {
            ships[i]->move(Vector2f(0.0f, 24.0f));
        }
    }
}

Player::Player() : Ship(IntRect(Vector2i(160, 32), Vector2i(32, 32)), ShipType::PLAYER) {
    setOrigin(Vector2f(16.f, 16.f));
    setTexture(spritesheet); // Ensure the texture is set
    setPosition({ gameWidth * .5f, gameHeight - 32.f });
}

void Player::Update(const float& dt) {
    Ship::Update(dt);

    // Move left
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        move(Vector2f(-200.0f * dt, 0.0f));
    }

    // Move right
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        move(Vector2f(200.0f * dt, 0.0f));
    }

    // Ensure the player stays within the game bounds
    if (getPosition().x < 0) {
        setPosition(0, getPosition().y);
    }
    if (getPosition().x > gameWidth - getGlobalBounds().width) {
        setPosition(gameWidth - getGlobalBounds().width, getPosition().y);
    }
}
