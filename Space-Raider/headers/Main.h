#ifndef MAIN_H
#define MAIN_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../headers/FPS.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

struct BulletType {
    std::string texture;
    int speed;
    int damge;
};

struct Bullet {
    sf::Sprite sprite;
    int speed;
    int damge;
};

enum class TextEffectTypes {
    INCREASE = 1,
    DECREASE = -1
};

struct TextEffect {
    sf::Text text;
    float maxPosition;
    sf::Uint8 red;
    sf::Uint8 green;
    sf::Uint8 blue;
    float opacity;
};
struct ColorStructre {
    int r, g, b;
};

const int APP_WIDTH = 800;
const int APP_HEIGHT = 600;
const float PI = 3.14159265;
float movingSpeed = 200.0f;
std::vector<BulletType> bulletLevels;
int maxBulletTypesCount = 0;
std::vector<Bullet> bullets;
std::vector<TextEffect> textEffects;
sf::Sprite player;
sf::Sprite cursor;
sf::Texture backgroundTexture;
sf::RectangleShape hpBar;
sf::RectangleShape hpBarBack;
sf::Font* font = new sf::Font;
sf::Text hpText;
int x = 0;
sf::Sprite background(backgroundTexture);
sf::Clock shootingThreashHold, animateTimer;
float deltaTime = 0;
float lastShootTime = 0, threashHold = 0.5, lastAnimateTime = 0;
int score = 0, powerLevel = 1, difficultyLevel = 1;
float health = 100, maxHealth = 100;
ColorStructre hpColor = {0, 255, 0};

void init();
void loadBulletLevels(std::string);
void movePlayer(float);
void rotatePlayer();
void shoot();
void handlePlayerEvents(float);
void setPowerLevel(int);
void increasePowerLevel();
void decreasePowerLevel();
void animateBackground();
void updateDifficulty();
void addTextEffect(sf::Text, float);
void newTextEffect(std::string, TextEffectTypes, sf::Vector2f);
void draw(sf::RenderWindow*);
void drawBullets(sf::RenderWindow*);
void drawTextEffects(sf::RenderWindow*);
void drawHelathBar(sf::RenderWindow*);
void updateHealthBar();
void increaseHealth(float);
void decreaseHealth(float);


#endif // !MAIN