#include "ship.h"
#include "game.h"
using namespace sf;
using namespace std;

bool Invader::direction;
float Invader::speed;

Ship::Ship() {};

Ship::Ship(IntRect ir) : Sprite() {
	_sprite = ir;
	setTexture(spritesheet);
	setTextureRect(_sprite);
};

void Ship::Update(const float& dt) {}

//Define the ship deconstructor. 
//Although we set this to pure virtual, we still have to define it.
Ship::~Ship() = default;

Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) {
	setOrigin(Vector2f(16.f, 16.f));;
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

Player::Player() : Ship(IntRect(Vector2i(160, 32), Vector2i(32, 32))) {
	setOrigin(Vector2f(16.f, 16.f));;
	setTexture(spritesheet); // Ensure the texture is set
	setPosition({ gameWidth * .5f, gameHeight - 32.f });
}

void Player::Update(const float& dt) {
	Ship::Update(dt);
	return;

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