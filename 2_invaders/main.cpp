#include <SFML/Graphics.hpp>
#include <iostream>
#include "ship.h"

using namespace sf;
using namespace std;

sf::Texture spritesheet;
sf::Sprite invader;
std::vector<Ship*> ships;

const int gameWidth = 800;
const int gameHeight = 600;

const int invaderWidth = 32;
const int invaderHeight = 32;
const int spacing = 10; // Space between invaders
const int invaders_rows = 5; // Number of rows of invaders
const int invaders_columns = 10; // Number of columns of invaders
const int firstColumnOffset = 20; // Offset for the first row

float invaderSpeed = 50.0f; // Speed of invaders
bool movingRight = true; // Direction of movement

void Load() {
    if (!spritesheet.loadFromFile("res/invaders_sheet.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }

    for (int r = 0; r < invaders_rows; ++r) {
        for (int c = 0; c < invaders_columns; ++c) {
            auto rect = IntRect(Vector2i(0, 0), Vector2i(invaderWidth, invaderHeight));
            Vector2f position = Vector2f(c * (invaderWidth + spacing) , r * (invaderHeight + spacing));
            auto inv = new Invader(rect, position);
            ships.push_back(inv);
        }
    }

    Player* player = new Player();
    ships.push_back(player);
}

void Render(RenderWindow& window) {
    window.draw(invader);
    
    for (const auto s : ships) {
        window.draw(*s);
    }
}

void Update(RenderWindow& window) {
    static Clock clock;
    float dt = clock.restart().asSeconds();

    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    // Update invaders' movement
    for (auto& ship : ships) {
        if (ship->getType() == ShipType::INVADER) {
            Vector2f position = ship->getPosition();
            if (movingRight) {
                position.x += invaderSpeed * dt;
            }
            else {
                position.x -= invaderSpeed * dt;
            }
            ship->setPosition(position);
        }
    }

    // Check for invaders hitting the screen edge and move them down
    for (auto& ship : ships) {
        if (ship->getType() == ShipType::INVADER) {
            Vector2f position = ship->getPosition();
            if ((movingRight && position.x + invaderWidth >= gameWidth) ||
                (!movingRight && position.x <= 0)) {
                movingRight = !movingRight;
                for (auto& invader : ships) {
                    if (invader->getType() == ShipType::INVADER) {
                        position = invader->getPosition();
                        position.y += invaderHeight / 2;  // Move invaders down
                        invader->setPosition(position);
                    }
                }
                break;
            }
        }
    }

    // Update player movement
    for (auto& ship : ships) {
        if (ship->getType() == ShipType::PLAYER) {
            ship->Update(dt);  // Call the player's Update method
        }

        // Quit Via ESC Key
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            for (auto s : ships) {
                delete s;
            }
            ships.clear();
            window.close();
        }
    }
}
int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "Space Invaders");
    Load();
    while (window.isOpen()) {
        Update(window);
        window.clear();

        Render(window);
        window.display();
    }
    return 0;
}