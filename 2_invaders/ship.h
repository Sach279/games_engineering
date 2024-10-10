#pragma once
#include <SFML/Graphics.hpp>

enum class ShipType {
    PLAYER,
    INVADER
};

class Ship : public sf::Sprite {
protected:
    sf::IntRect _sprite;
    ShipType type;  // Add type property
    Ship(); // Default constructor hidden
public:
    explicit Ship(sf::IntRect ir, ShipType shipType); // Modify constructor
    virtual ~Ship() = 0;
    virtual void Update(const float& dt);
    virtual void MoveDown(float distance) {
        move(0, distance);
    }

    ShipType getType() const { return type; } // Getter for the type
};

// Existing classes
class Invader : public Ship {
public:
    static bool direction;
    static float speed;
    Invader(sf::IntRect ir, sf::Vector2f pos);
    Invader();
    void Update(const float& dt) override;
};

class Player : public Ship {
public:
    Player();
    void Update(const float& dt) override;
    void MoveDown(float distance) override {
        // Do nothing, player doesn't move down
    }
};
