#include "./headers/Main.h"
#include "./headers/Time.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(APP_WIDTH, APP_HEIGHT), "Space Raider");
    window.setMouseCursorVisible(false); // To Hide The Default Cursor
    window.setKeyRepeatEnabled(false); // To Register The Keyboard input only once at frame
    
    sf::Clock clock;
    FPS fps;

    // Start: Files To Load Before Run
    loadBulletLevels("bulletLevels");
    // End: Files To Load Before Run
    
    init(); // Intinalizing Assets

    // Start: Intervals
    setInterval(updateDifficulty , 3600);
    // End: Intervals

    // Start: Game Loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            // For Debug Only
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                increasePowerLevel();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                decreasePowerLevel();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
                newTextEffect("10", TextEffectTypes::INCREASE, static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
                newTextEffect("10", TextEffectTypes::DECREASE, static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                decreaseHealth(5);
        }
        //std::cout << "FPS: " + std::to_string(fps.getFPS()) + "\n";
        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
        window.clear();
        // draw everything here...
         draw(&window);
         handlePlayerEvents(deltaTime);
         animateBackground();
         fps.update(); // Call fps to be updated every farme otherwise it will always be 0
         deltaTime = clock.restart().asSeconds();
         // end the current frame
        window.display();
    }
    // end: Game Loop
    return 0;
}


void init() {
    // Start: Font
        font->loadFromFile("./fonts/kenvector_future.ttf");
    // End: Font
    // Start: Background
    backgroundTexture.setRepeated(true);
    backgroundTexture.loadFromFile("textures/background.png");
    background.setTextureRect(sf::IntRect(0, 0, 800, 600));
    // End: Background

    // Start: Cursor
    sf::Texture* cursorTexture = new sf::Texture;
    cursorTexture->loadFromFile("textures/cursor.png");
    cursor.setTexture(*cursorTexture);
    cursor.setOrigin(cursorTexture->getSize().x / 2.0, cursorTexture->getSize().y / 2.);
    // End: Cursor
    // Start: Player
    sf::Texture* playerTexture = new sf::Texture;
    playerTexture->loadFromFile("textures/player/jet0.png");
    player.setTexture(*playerTexture);
    player.setOrigin(sf::Vector2f(playerTexture->getSize().x / 2.0, playerTexture->getSize().y / 2.0));
    player.setPosition(sf::Vector2f(playerTexture->getSize().x / 2.0, APP_HEIGHT / 2.0));
    // End: Player

    // Start: Player HP Bar
    hpBar.setSize(sf::Vector2f(50, 3.f));
    hpBar.setFillColor(sf::Color(hpColor.r, hpColor.g, hpColor.b));
    hpBar.setPosition(sf::Vector2f(player.getPosition().x - player.getTexture()->getSize().x + 60, player.getPosition().y - player.getTexture()->getSize().y + 35));
    hpBarBack = hpBar;
    hpBarBack.setFillColor(sf::Color(25, 25, 25, 200));
    hpBarBack.setOutlineColor(sf::Color(255, 255, 255, 100));
    hpBarBack.setOutlineThickness(2);
    hpText.setString("HP: " + std::to_string((int)health));
    hpText.setFillColor(sf::Color(255, 255, 255, 100));
    hpText.setPosition(sf::Vector2f(player.getPosition().x - player.getTexture()->getSize().x + 60, player.getPosition().y - player.getTexture()->getSize().y + 10));    hpText.setFont(*font);
  
    hpText.setCharacterSize(12);
    // End: Player HP Bar
};

void loadBulletLevels(std::string fileName) {
    std::ifstream loadBulletLevelsFile("./loaders/" + fileName + ".txt");
    if(loadBulletLevelsFile.is_open()) {
        std::string line, texture;
        int speed, damage;
        while (std::getline(loadBulletLevelsFile, line)) {
            std::istringstream in(line);
            in >> texture >> speed >> damage;
            bulletLevels.push_back({texture, speed, damage });
            maxBulletTypesCount++;
        }
    }
    loadBulletLevelsFile.close();
};

void movePlayer(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        if (player.getPosition().y + player.getTexture()->getSize().y / 2 + 10 < APP_HEIGHT)
            player.move(0, movingSpeed * deltaTime);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        if (player.getPosition().y - player.getTexture()->getSize().y / 2 - 10 > 0)
            player.move(0, -movingSpeed * deltaTime);
}

void rotatePlayer() {
    sf::Vector2f playerPostion = player.getPosition();
    float dx = playerPostion.x - cursor.getPosition().x;
    float dy = playerPostion.y - cursor.getPosition().y;
    float rotation = (atan2(dy, dx)) * 180 / PI;
    player.setRotation(rotation - 90);
}

void shoot() {
    BulletType currentBulletType = bulletLevels[powerLevel - 1];
    float cosX = cos((90 - player.getRotation()) * (PI / 180));
    float sinX = sin((90 - player.getRotation()) * (PI / 180));
    sf::Texture *bulletTexture = new sf::Texture;
    bulletTexture->loadFromFile("./textures/laser/" + currentBulletType.texture +".png");
    sf::Sprite bullet(*bulletTexture);
    bullet.setRotation(player.getRotation());
    bullet.setOrigin(bulletTexture->getSize().x, bulletTexture->getSize().y);
    bullet.setPosition(player.getPosition().x + (cosX * player.getTexture()->getSize().x / 2), player.getPosition().y - (sinX * player.getTexture()->getSize().y / 2));
    bullets.push_back({ bullet, currentBulletType.speed, currentBulletType.damge });
    lastShootTime = shootingThreashHold.restart().asSeconds();
}

void handlePlayerEvents(float deltaTime) {
    if (abs(shootingThreashHold.getElapsedTime().asSeconds() - lastShootTime) >= threashHold) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            shoot();
    }
    movePlayer(deltaTime);
    rotatePlayer();
    increaseHealth(0.001);
    updateHealthBar();
}

void increasePowerLevel() {
    if (powerLevel >= maxBulletTypesCount) return;
    powerLevel++;
};

void decreasePowerLevel() {
    if (powerLevel <= 1) return;
    powerLevel--;
};

void setPowerLevel(int level) {
    if (level <= 0 || level >= maxBulletTypesCount) return;
    powerLevel = level;
};

void animateBackground() {
    if (abs(animateTimer.getElapsedTime().asSeconds() - lastAnimateTime) >= 0.05) {
        x += 2;
        background.setTextureRect(sf::IntRect(x, x, 800, 600));
        lastShootTime = animateTimer.restart().asSeconds();
    }
};

void updateDifficulty() {
    difficultyLevel = (score % 100) ? score / 100 + 1 : score / 100;
};

void newTextEffect(std::string textToShow, TextEffectTypes type, sf::Vector2f startPosition) {
    sf::Text text;
    text.setFont(*font);
    text.setString(((type == TextEffectTypes::INCREASE) ? "+" : "-") + textToShow);
    text.setFillColor(((type == TextEffectTypes::INCREASE) ? sf::Color(50, 205, 50, 255) : sf::Color(255, 0, 56, 255)));
    text.setPosition(startPosition);
    text.setCharacterSize(24);
    addTextEffect(text, startPosition.y - 100);
};

void addTextEffect(sf::Text text, float maxPosition) {
    textEffects.push_back({ text, maxPosition, text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, (float)text.getFillColor().a });
}

void draw(sf::RenderWindow* window) {
    window->draw(background); // Drawing Background
    window->draw(cursor); // Drawing Cursor
    window->draw(player); // Drawing Player
    drawBullets(window);
    drawTextEffects(window);
    drawHelathBar(window);
};

void drawBullets(sf::RenderWindow* window) {
    for (unsigned int i = 0; i < bullets.size(); i++) {
        // Will be Draw only if the bullet is in the screen bound
        if (bullets[i].sprite.getPosition().x < APP_WIDTH && bullets[i].sprite.getPosition().y < APP_HEIGHT) {
            bullets[i].sprite.move(sin(bullets[i].sprite.getRotation() * PI / 180) * bullets[i].speed * deltaTime, -cos(bullets[i].sprite.getRotation() * PI / 180) * bullets[i].speed * deltaTime);
            window->draw(bullets[i].sprite);
        }
        else // if it went out the screen bound it will be delete
            bullets.erase(bullets.begin() + i);
    } // Bulltes Logic and Drawign
};

void drawTextEffects(sf::RenderWindow* window) {
    for (unsigned int i = 0; i < textEffects.size(); i++) {
        // Will be Draw only if the TextEffect is in the Moving Range
        if (textEffects[i].text.getPosition().y > textEffects[i].maxPosition) {
            textEffects[i].text.move(0, -100 * deltaTime);
            textEffects[i].opacity -= (textEffects[i].opacity > 0) ? 0.1 : 0;
            textEffects[i].text.setFillColor(sf::Color(textEffects[i].red, textEffects[i].green, textEffects[i].blue, (sf::Uint8)textEffects[i].opacity));
            window->draw(textEffects[i].text);
        }
        else // if it went out the Range it will be delete
            textEffects.erase(textEffects.begin() + i);
    } // Score Text Effect Logic and Drawign
};

void drawHelathBar(sf::RenderWindow* window) {
    window->draw(hpBarBack);
    window->draw(hpBar);
    window->draw(hpText);
}

void updateHealthBar() {
    hpBar.setSize(sf::Vector2f((health / maxHealth) * 50, hpBar.getSize().y));
    hpBar.setPosition(sf::Vector2f(player.getPosition().x - player.getTexture()->getSize().x + 60, player.getPosition().y - player.getTexture()->getSize().y + 35));
    hpBarBack.setPosition(sf::Vector2f(player.getPosition().x - player.getTexture()->getSize().x + 60, player.getPosition().y - player.getTexture()->getSize().y + 35));
    hpText.setString("HP: " + std::to_string((int)health));
    hpText.setPosition(sf::Vector2f(player.getPosition().x - player.getTexture()->getSize().x + 60, player.getPosition().y - player.getTexture()->getSize().y + 10));
    hpColor.r = 2.55 * (((100.0 - health) / maxHealth) * 100.0);
    hpColor.g = 2.55 * ((health / maxHealth) * 100.0);
    hpBar.setFillColor(sf::Color((sf::Uint8)hpColor.r, (sf::Uint8)hpColor.g, (sf::Uint8)hpColor.b));
};

void increaseHealth(float amount) {
    if (health >= maxHealth) return;
    health += amount;
    if (health > maxHealth) health = maxHealth;
};

void decreaseHealth(float amount) {
    if (health - amount <= 0) return;
    health -= amount;
};
