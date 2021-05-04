#ifndef MAIN_H
#define MAIN_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "./headers/FPS.h"
#include "./headers/Menu.h"
#include "./headers/SoundManager.h"
#include "./headers/Storage.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <ctime>

sf::Clock totalTime;
const sf::Vector2f APP_SIZE = { 800, 600 };
sf::ContextSettings settings;
const double PI = 3.14159265;
const int MAX = 5;
int maxBulletTypesCount = 0;
std::map<std::string, std::map<std::string, std::vector<int>>> meteorTypes;
sf::Sprite cursor;
sf::Font* font = new sf::Font;
sf::Texture backgroundTexture;
sf::Sprite background(backgroundTexture);
int backgroundAnimationCounter = 0;
sf::Clock animateTimer;
float deltaTime = 0;
float lastAnimateTime = 0;
int difficultyLevel = 1, selectedShape = 1, selectedColor = 0;
bool canSpawnLuckyMeteor = false, canSpawnLuckyUFO = false;
std::string UFOColors[3] = { "blue", "green", "red" };
std::string EnemiesColors[4] = { "blue", "green", "gray", "orange" };
std::string colors[2] = { "brown", "gray" };
std::string sizes[3] = { "small", "medium", "big" };
std::string playerColors[4] = { "blue", "green", "orange", "red"  };

SoundManager soundManager("music", "sfx");

int metorsDestroied = 0, goldenMetorsDestroied = 0, ufosDestroied = 0;
sf::Vector2f findPath(sf::Vector2f, sf::Sprite);

enum class TextEffectTypes {
    INCREASE = 1,
    DECREASE = -1
};

struct BulletType {
    std::string textureFile;
    int speed;
    int damage;
};

struct Bullet {
    sf::Sprite sprite;
    int speed;
    int damage;
};

struct TextEffect {
    sf::Text text;
    float maxPosition;
    sf::Uint8 red;
    sf::Uint8 green;
    sf::Uint8 blue;
    float opacity;
};

struct ColorStructure {
    double red, green, blue;
};

struct Meteor {
    sf::Sprite* sprite;
    sf::Vector2f linearPath;
    std::string size;
    std::string color;
    int speed;
    int scoreToGain;
    bool isDividable;
    bool isPathFound;
    bool isGolden;
    int framesCount;
    int lastFrame;
    sf::Clock goldenMeteorTimer;
    void animate() { //Add animation to a sprite 
        if (goldenMeteorTimer.getElapsedTime().asSeconds() >= 0.1) {
            lastFrame++;
            goldenMeteorTimer.restart();
        }
        sprite->setTextureRect(sf::IntRect((sprite->getTexture()->getSize().x / framesCount) * lastFrame, 0 * lastFrame, (sprite->getTexture()->getSize().x / framesCount), sprite->getTexture()->getSize().y));
        lastFrame %= framesCount - 1;
    }
};

struct UnidentifiedFlyingObject {
    sf::Texture* texture = new sf::Texture;
    sf::Texture bulletTexture;
    int bulletDamage = 10;
    sf::Sprite sprite;
    sf::Vector2f linearPath;
    std::vector<sf::Sprite> bullets;
    sf::Clock timer, circularMotionTimer, bulletsTimer;
    bool isDone, hasArrived;
    int speed;

    UnidentifiedFlyingObject() :speed(100), isDone(false), hasArrived (false) {
        std::string color = UFOColors[(std::rand() % (sizeof(UFOColors) / sizeof(UFOColors[0])))];
        texture->loadFromFile("./textures/ufo/" + color + ".png");
        bulletTexture.loadFromFile("./textures/laser/" + color + "-1.png");
        sprite.setTexture(*texture);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
        sprite.setPosition(840, 640);
        linearPath = findPath(sf::Vector2f(APP_SIZE.x / 2, APP_SIZE.y / 2), sprite);
        timer.restart();
        circularMotionTimer.restart();
    };
    void draw(sf::RenderWindow* window) {
        window->draw(sprite);
    }
    void move() {
        if (sprite.getPosition().x > (APP_SIZE.x / 2) && !hasArrived) {
            sprite.move(linearPath.x * speed * deltaTime, linearPath.y * speed * deltaTime);
            sprite.rotate(0.05);
        }
        else if (timer.getElapsedTime().asSeconds() <= 3) {
            hasArrived = true;
            shot();
        }
        else if (timer.getElapsedTime().asSeconds() <= 5.5) {
            sprite.move(speed * deltaTime, -speed * deltaTime);
            sprite.rotate(0.03);
        }
        else if (timer.getElapsedTime().asSeconds() > 5.5) {
            sf::Vector2f newPath = findPath(sf::Vector2f(880, 0), sprite);
            sprite.move(newPath.x * speed * deltaTime, newPath.y * speed * deltaTime);
            sprite.rotate(0.05);
        }
        else if (timer.getElapsedTime().asSeconds() >= 7) {
            sprite.setScale(0, 0);
            isDone = true;
        }
    };
    void drawBullets(sf::RenderWindow* window) {
        if (hasArrived) {
            for (int i = 0; i < this->bullets.size(); i++) {
                this->bullets[i].move((float)sin(this->bullets[i].getRotation() * PI / 180) * speed * deltaTime, (float)-cos(this->bullets[i].getRotation() * PI / 180) * speed * deltaTime);
                window->draw(this->bullets[i]);
            }
        }
    };
    void shot() {
        if (bulletsTimer.getElapsedTime().asSeconds() >= 1.25) {
            sf::Sprite bullet(bulletTexture);
            for (int i = 0; i < 3; i++) {
                bullet.setPosition(sprite.getPosition().x - (sprite.getTextureRect().width / 2) - 6 * i, sprite.getPosition().y + 6 * i);
                bullet.setOrigin(bullet.getTextureRect().width, bullet.getTextureRect().height);
                bullet.setRotation(-45 * (i + 1));
                this->bullets.push_back(bullet);
            }
            bulletsTimer.restart();
        }
    };
};
std::vector<BulletType> bulletLevels;
void newFloatingText(std::string, TextEffectTypes, sf::Vector2f);
bool isInScreenBounds(sf::Sprite);
struct Player {
    int score = 0, powerLevel = 1, lives = 3;
    double health = 100, maxHealth = 100;
    float lastShootTime = 0, threshHold = 0.5;
    float movingSpeed = 200.0f;
    sf::Clock shootingThreshHold;
    sf::RectangleShape hpBar, hpBarBack;
    sf::Text hpText;
    sf::Sprite sprite;
    std::vector<Bullet> bullets;
    ColorStructure hpColor = { 0, 255, 0 };

    Player(std::string texturePath) {
        // Start: Player
        sf::Texture* playerTexture = new sf::Texture;
        playerTexture->loadFromFile("./textures/player/" + texturePath);
        sprite.setTexture(*playerTexture);
        sprite.setOrigin(sf::Vector2f((float)playerTexture->getSize().x / 2.f, (float)playerTexture->getSize().y / 2.f));
        sprite.setPosition(sf::Vector2f(playerTexture->getSize().x / 2.f, APP_SIZE.y / 2.f));
        // End: Player

        // Start: Player HP Bar
        hpBar.setSize(sf::Vector2f(50, 3.f));
        hpBar.setFillColor(sf::Color((sf::Uint8)hpColor.red, (sf::Uint8)hpColor.green, (sf::Uint8)hpColor.blue));
        hpBar.setPosition(sf::Vector2f(sprite.getPosition().x - sprite.getTexture()->getSize().x + 60, sprite.getPosition().y - sprite.getTexture()->getSize().y + 35));
        hpBarBack = hpBar;
        hpBarBack.setFillColor(sf::Color(25, 25, 25, 200));
        hpBarBack.setOutlineColor(sf::Color(255, 255, 255, 100));
        hpBarBack.setOutlineThickness(2);
        hpText.setString("HP: " + std::to_string((int)health));
        hpText.setFillColor(sf::Color(255, 255, 255, 100));
        hpText.setPosition(sf::Vector2f(sprite.getPosition().x - sprite.getTexture()->getSize().x + 60, sprite.getPosition().y - sprite.getTexture()->getSize().y + 10));
        hpText.setFont(*font);
        hpText.setCharacterSize(12);
        // End: Player HP Bar
    };

    void update() {
        if (abs(shootingThreshHold.getElapsedTime().asSeconds() - lastShootTime) >= threshHold) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                shoot();
        }
        move();
        rotate();
        increaseHealth(0.001);
        updateHealthBar();
    };

    void draw(sf::RenderWindow* window) {
        window->draw(sprite);
        drawHealthBar(window); // Rendring Health bar
        if (bullets.size() > 0) drawBullets(window); // Rendring Bullets if player have shoot any
    };

    void drawBullets(sf::RenderWindow* window) {
        for (unsigned int i = 0; i < bullets.size(); i++) {
            // Will be Draw only if the bullet is in the screen bound
            if (isInScreenBounds(bullets[i].sprite)) {
                bullets[i].sprite.move((float)sin(bullets[i].sprite.getRotation() * PI / 180) * bullets[i].speed * deltaTime, (float)-cos(bullets[i].sprite.getRotation() * PI / 180) * bullets[i].speed * deltaTime);
                window->draw(bullets[i].sprite);
            }
            else // if it went out the screen bound it will be delete
                bullets.erase(bullets.begin() + i);
        } // Bulltes Logic and Drawign
    };

    void drawHealthBar(sf::RenderWindow* window) {
        window->draw(hpBarBack);
        window->draw(hpBar);
        window->draw(hpText);
    };

    void move() {
        int direction = 0;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sprite.getPosition().y + sprite.getTexture()->getSize().y / 2 + 10 < APP_SIZE.y)
            direction += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sprite.getPosition().y - sprite.getTexture()->getSize().y / 2 - 10 > 0)
            direction += -1;
        sprite.move(0, direction * movingSpeed * deltaTime);
        direction = 0;
    }

    void rotate() {
        sf::Vector2f playerPostion = sprite.getPosition();
        float dx = playerPostion.x - cursor.getPosition().x;
        float dy = playerPostion.y - cursor.getPosition().y;
        float rotation = (float)(atan2(dy, dx) * (180 / PI));
        sprite.setRotation(rotation - 90.f);
    }

    void shoot() {
        BulletType currentBulletType = bulletLevels[powerLevel - 1];
        float cosX = cos((float)((90.f - sprite.getRotation()) * (PI / 180)));
        float sinX = sin((float)((90.f - sprite.getRotation()) * (PI / 180)));
        sf::Texture* bulletTexture = new sf::Texture;
        bulletTexture->loadFromFile("./textures/laser/" + currentBulletType.textureFile + ".png");
        sf::Sprite bullet(*bulletTexture);
        bullet.setRotation(sprite.getRotation());
        bullet.setOrigin((float)bulletTexture->getSize().x, (float)bulletTexture->getSize().y);
        bullet.setPosition(sprite.getPosition().x + (cosX * sprite.getTexture()->getSize().x / 2), sprite.getPosition().y - (sinX * sprite.getTexture()->getSize().y / 2));
        bullets.push_back({ bullet, currentBulletType.speed, currentBulletType.damage });
        soundManager.playSFX("shooting");
        lastShootTime = shootingThreshHold.restart().asSeconds();
    }

    void increasePowerLevel() {
        if (powerLevel >= maxBulletTypesCount) return;
        powerLevel++;
        soundManager.playSFX("PowerUp");
        newFloatingText("Power Level Increased", TextEffectTypes::INCREASE, sf::Vector2f(250, 150));
    };

    void decreasePowerLevel() {
        if (powerLevel <= 1) return;
        powerLevel--;
        newFloatingText("Power Level Decreased", TextEffectTypes::DECREASE, sf::Vector2f(250, 150));
    };

    void takeDamge(int amount) {
        health -= amount;
        soundManager.playSFX("TakeDMG");
        if (health <= 0) {
            if (lives > 0) {
                lives--;
                soundManager.playSFX("Lost_one_life");
                health = 100;
            }
        }
    };

    void updateHealthBar() {
        hpBar.setSize(sf::Vector2f(((float)health / (float)maxHealth) * 50.f, hpBar.getSize().y));
        hpBar.setPosition(sf::Vector2f(sprite.getPosition().x - sprite.getTexture()->getSize().x + 60, sprite.getPosition().y - sprite.getTexture()->getSize().y + 35));
        hpBarBack.setPosition(sf::Vector2f(sprite.getPosition().x - sprite.getTexture()->getSize().x + 60, sprite.getPosition().y - sprite.getTexture()->getSize().y + 35));
        hpText.setString("HP: " + std::to_string((int)health));
        hpText.setPosition(sf::Vector2f(sprite.getPosition().x - sprite.getTexture()->getSize().x + 60, sprite.getPosition().y - sprite.getTexture()->getSize().y + 10));
        hpColor.red = 2.55 * (((100.0 - health) / maxHealth) * 100.0);
        hpColor.green = 2.55 * ((health / maxHealth) * 100.0);
        hpBar.setFillColor(sf::Color((sf::Uint8)hpColor.red, (sf::Uint8)hpColor.green, (sf::Uint8)hpColor.blue));
    };

    void increaseHealth(double amount) {
        if (health >= maxHealth) return;
        health += amount;
        if (health > maxHealth) health = maxHealth;
    };

    void decreaseHealth(double amount) {
        if (health - amount <= 0) return;
        health -= amount;
    };

};

Player player(std::to_string(selectedShape) + "/" + playerColors[selectedColor] + ".png");

struct Enemy {
    int objectNumber;
    int hp;
    int bulletSpeed;
    int movingSpeed;
    int bulletDamage = 5;
    sf::Texture* texture = new sf::Texture;
    sf::Sprite sprite;
    sf::Vector2f linearPath;
    std::vector<sf::Sprite> bullets;
    sf::Clock shootingThreshHold;
    float threshold = 5;
    int fixedDistance = 0;

    Enemy(int position, int _objectNumber) :hp(100), bulletSpeed(100), movingSpeed(200) {
        objectNumber = _objectNumber;
        int index = 1 + (std::rand() % 5);
        std::string color = EnemiesColors[(std::rand() % (sizeof(EnemiesColors) / sizeof(EnemiesColors[0])))];
        texture->loadFromFile("./textures/enemy/" + color + "/" + std::to_string(index) + ".png");
        sprite.setTexture(*texture);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
        sprite.setScale(0.75, 0.75);
        sprite.setPosition((std::rand() % 200) + APP_SIZE.x + 200 * position, std::rand() % ((int)APP_SIZE.y / 2));
        fixedDistance = 800 - (100 * position);
        linearPath = findPath(sf::Vector2f(800 - (objectNumber * 80), std::rand() % ((int)APP_SIZE.y / 2)), sprite);
    };
    void draw(sf::RenderWindow* window) {
        window->draw(sprite);
    }
    void changeDirection() {
        movingSpeed = -movingSpeed;
    }
    void TakePlace() {
        if (sprite.getPosition().x <= fixedDistance) {
            if (sprite.getPosition().y >= 550 ||sprite.getPosition().y <= 0)
                changeDirection();
            sprite.move(0, movingSpeed * deltaTime);
        }
        else 
            sprite.move(linearPath.x * movingSpeed * deltaTime, linearPath.y * movingSpeed * deltaTime);
    }
    void upgrade(int difficulty) {
        hp += difficulty * 10;
        bulletSpeed += difficulty / 3;
    }
    void rotate() {
        float dx = sprite.getPosition().x - player.sprite.getPosition().x;
        float dy = sprite.getPosition().y - player.sprite.getPosition().y;
        float rotation = (float)(atan2(dy, dx) * (180 / PI));
        sprite.setRotation(rotation + 90);
    }
    void shoot() {
        if (shootingThreshHold.getElapsedTime().asSeconds() >= threshold + objectNumber) {
            float cosX = cos((float)((90.f - sprite.getRotation()) * (PI / 180)));
            float sinX = sin((float)((90.f - sprite.getRotation()) * (PI / 180)));
            sf::Texture* bulletTexture = new sf::Texture;
            bulletTexture->loadFromFile("./textures/laser/red-1.png");
            sf::Sprite bullet(*bulletTexture);
            bullet.setRotation(sprite.getRotation());
            bullet.setOrigin((float)bulletTexture->getSize().x, 0);
            bullet.setPosition(sprite.getPosition().x - (cosX * sprite.getTexture()->getSize().x / 2), sprite.getPosition().y + (sinX * sprite.getTexture()->getSize().y / 2));
            bullets.push_back(bullet);
            shootingThreshHold.restart();
        }
    }
};

struct PlayerName {
    std::string name;
    int length;
} playerName = { "", 0 };


std::vector<UnidentifiedFlyingObject> UFOs;
std::vector<Meteor> meteors;
std::vector<TextEffect> textEffects;
std::vector<Enemy> enemies;

void newEnemy() {
    for (int i = 0; i < MAX; i++) {
        if (enemies.size() < MAX) {
            Enemy enemy = Enemy(i + 1, enemies.size() + 1);
            enemies.push_back(enemy);
        }
    }
}

void handleEnemy(sf::RenderWindow*);
void init();
void loadBulletLevels(std::string);
void loadMeteors(std::string);
void spawnMeteors();
void handleAllEnemies(sf::RenderWindow*);
void animateBackground();
void updateDifficulty();
void addTextEffect(sf::Text, float);
void newTextEffect(int, TextEffectTypes, sf::Vector2f);
void draw(sf::RenderWindow*);
void drawTextEffects(sf::RenderWindow*);
void drawMeteors(sf::RenderWindow*);
void lucky(); // spawn a Lucky Meteor Or a Ufo Randomly Or maybe both
#define VIDEOMODE  sf::VideoMode(APP_SIZE.x, APP_SIZE.y)
std::string TITLE = "Space Raidedr";
sf::Image icon;
Storage storage;

void setupLogo(sf::RenderWindow& window) {
    icon.loadFromFile("./textures/logo.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
};

void switchWindows(sf::Mutex& mutex, sf::RenderWindow& window, sf::Thread& thread) {
    mutex.lock();
    window.close();
    thread.launch();
    mutex.unlock();
};

void handleBasicEvents(sf::Event& event, sf::RenderWindow& window, Menu& menu) {
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::KeyPressed:
            soundManager.playSFX("menu");
            menu.handleEvents(event);
        }
    }
}
void mainMenu();
void gameLoop();
void instructions();
void leaderBoard();
void credits();
void enterYourName();
void lose();
void choosePlayerShape();
void choosePlayerColor();

sf::Mutex mainMenuMutex, gameMutex,
instructionsMutex, leaderBoardMutex,
creditsMutex, enterYourNameMutex, loseMutex, choosePlayerShapeMutex, choosePlayerColorMutex;

#endif // !MAIN