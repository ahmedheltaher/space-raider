#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

const int APP_WIDTH = 800;
const int APP_HEIGHT = 600;
float movingSpeed = 200.0f;
std::vector<sf::Sprite> bullets;
sf::Sprite player;
sf::Texture bulletTexture;
sf::Clock shootingThreashHold;
float lastShootTime = 0, threashHold = 0.5;

void movePlayer(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        if (player.getPosition().y + player.getTexture()->getSize().y / 2 + 10 < APP_HEIGHT)
            player.move(0, movingSpeed * deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            if (player.getPosition().y - player.getTexture()->getSize().y / 2 - 10 > 0)
                player.move(0, -movingSpeed * deltaTime);
}

void shoot() {
    bulletTexture.loadFromFile("textures/laser/laserBlue01.png");
    sf::Sprite bullet(bulletTexture);
    bullet.setRotation(90.0);
    bullet.setPosition(player.getPosition().x + player.getTexture()->getSize().x + 5, player.getPosition().y - bulletTexture.getSize().x / 2);
    bullets.push_back(bullet);
    lastShootTime = shootingThreashHold.restart().asSeconds();
}

void handlePlayerEvents(float deltaTime) {
    if (abs(shootingThreashHold.getElapsedTime().asSeconds() - lastShootTime) >= threashHold) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            shoot();
    }
    movePlayer(deltaTime);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(APP_WIDTH, APP_HEIGHT), "Space Raider");
    float deltaTime = 0;

    // Start Background
    sf::Texture backgroundTexture;
    backgroundTexture.setRepeated(true);
    backgroundTexture.loadFromFile("textures/background.png");
    sf::Sprite background(backgroundTexture);
    background.setTextureRect(sf::IntRect(0, 0, 5000, 5000));
    // End Background

    // Start player
    sf::Texture playerTexture;
    playerTexture.loadFromFile("textures/player/jet0.png");
    player.setTexture(playerTexture);
    player.setOrigin(sf::Vector2f(playerTexture.getSize().x / 2.0, playerTexture.getSize().y / 2.0));
    player.setPosition(sf::Vector2f(playerTexture.getSize().x / 2.0, APP_HEIGHT / 2.0));
    player.setRotation(90.0);
    // End Player

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        // draw everything here...
         window.draw(background); // Drawing Background
         window.draw(player); // Drawing Player
         for (int i = 0; i < bullets.size(); i++) {
             if (bullets[i].getPosition().x < APP_WIDTH) {
                 bullets[i].move(200 * deltaTime, 0);
                 window.draw(bullets[i]);
             }
             else
                 bullets.erase(bullets.begin() + i);
         }
         handlePlayerEvents(deltaTime);
         deltaTime = clock.restart().asSeconds();
         // end the current frame
        window.display();
    }
    return 0;
}