#ifndef ENEMY_H
#define ENEMY_H

struct Enemy {
    int objectNumber;
    int hp;
    int bulletSpeed;
    int bulletDamage = 5;
    int movingSpeed;
    sf::Texture* texture = new sf::Texture;
    sf::Sprite sprite;
    sf::Vector2f linearPath;
    std::vector<Bullet> bullets;
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

    void update(sf::RenderWindow* window, float deltaTime,int i, Player player, std::vector<Enemy> enemies) {
        draw(window);
        TakePlace(deltaTime);
        rotate();
        shoot();
        for (unsigned int j = 0; j < bullets.size(); j++) {
           bullets[j].move(deltaTime, -1);
            window->draw(bullets[j].sprite);
            if (bullets[j].sprite.getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
                player.takeDamge(bulletDamage);
                player.decreasePowerLevel();
                bullets.erase(bullets.begin() + j);
                for (unsigned int k = 0; k < enemies.size(); k++)
                    enemies[k].bullets.clear();
                break;
            }
        }
        for (unsigned int j = 0; j < player.bullets.size(); j++) {
            if (sprite.getGlobalBounds().intersects(player.bullets[j].sprite.getGlobalBounds())) {
                newTextEffect(35, TextEffectTypes::INCREASE, sprite.getPosition());
                player.score += 35;
                player.bullets.erase(player.bullets.begin() + j);
                enemies.erase(enemies.begin() + i);
                break;
            }
        }
    };

    void draw(sf::RenderWindow* window) {
        window->draw(sprite);
    }
    void changeDirection() {
        movingSpeed = -movingSpeed;
    }
    void TakePlace(float deltaTime) {
        if (sprite.getPosition().x <= fixedDistance) {
            if (sprite.getPosition().y >= 550 || sprite.getPosition().y <= 0)
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
        float dx = sprite.getPosition().x - sprite.getPosition().x;
        float dy = sprite.getPosition().y - sprite.getPosition().y;
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
            bullets.push_back({ bullet, bulletSpeed, bulletDamage });
            shootingThreshHold.restart();
        }
    }
};

#endif // !ENEMY