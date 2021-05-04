#include "./headers/Main.h"
#include "./headers/Time.h"


void mainMenu() {
    mainMenuMutex.lock();
    sf::RenderWindow mainMenuWindow(VIDEOMODE, TITLE);
    setupLogo(mainMenuWindow);
    sf::Thread gameThread(&gameLoop),
        instructionsThread(&instructions), leaderBoardThread(&leaderBoard),
        creditsThread(&credits), enterYourNameThread(&enterYourName);
    Menu menu("kenvector_future", 24, 100, 100, 400, 400, true);
    menu.addNewItem("Play", [&]() { switchWindows(enterYourNameMutex, mainMenuWindow, enterYourNameThread); });
    menu.addNewItem("Instruction", [&]() { switchWindows(instructionsMutex, mainMenuWindow, instructionsThread); });
    menu.addNewItem("Leader Board", [&]() { switchWindows(leaderBoardMutex, mainMenuWindow, leaderBoardThread); });
    menu.addNewItem("Credits", [&]() { switchWindows(creditsMutex, mainMenuWindow, creditsThread); });
    menu.addNewItem("Exit", [&]() { mainMenuWindow.close(); });

    sf::Texture imagetexture;
    imagetexture.loadFromFile("./textures/jet4.png");
    imagetexture.setSmooth(true);
    sf::Sprite imageSprite(imagetexture);
    imageSprite.setScale((float)1.5, (float)1.5);
    imageSprite.setPosition(sf::Vector2f(480, 250));

    while (mainMenuWindow.isOpen()) {
        sf::Event event;
        handleBasicEvents(event, mainMenuWindow, menu);
        mainMenuWindow.clear();
        menu.draw(&mainMenuWindow);
        mainMenuWindow.draw(imageSprite);
        mainMenuWindow.display();
    }
    mainMenuMutex.unlock();
};

void instructions() {
    instructionsMutex.lock();
    sf::Thread mainMenuThread(&mainMenu);
    sf::RenderWindow instructionsWindow(VIDEOMODE, TITLE + " - Instructions");
    setupLogo(instructionsWindow);
    Menu controlMenu("kenvector_future", 24, 500, 100, 400, 400, true);
    controlMenu.addNewItem("Back", [&]() { switchWindows(mainMenuMutex, instructionsWindow, mainMenuThread); });
    controlMenu.addNewItem("Exit", [&]() { instructionsWindow.close(); });
    Menu textMenu("kenvector_future", 30, 0, 100, 400, 700, false);
    textMenu.addNewItem("\t\tGoaL\nGuide Mario around \nthe Map and go to \nthe end of the\nmap whilet avoiding \nthose nasty Goombas.\n \t  Contols\nWASD keys for Mario \nEsc : Pause game \n ", []() {});

    while (instructionsWindow.isOpen()) {
        sf::Event event;
        handleBasicEvents(event, instructionsWindow, controlMenu);
        instructionsWindow.clear();
        controlMenu.draw(&instructionsWindow);
        textMenu.draw(&instructionsWindow);
        instructionsWindow.display();
    }
    instructionsMutex.unlock();
};

void leaderBoard() {
    leaderBoardMutex.lock();
    sf::Thread mainMenuThread(&mainMenu);
    sf::RenderWindow leaderBoardWindow(VIDEOMODE, TITLE + " - Leader Board");
    setupLogo(leaderBoardWindow);
    Menu controlMenu("kenvector_future", 24, 500, 100, 400, 400, true);
    controlMenu.addNewItem("Back", [&]() { switchWindows(mainMenuMutex, leaderBoardWindow, mainMenuThread); });
    controlMenu.addNewItem("Exit", [&]() { leaderBoardWindow.close(); });
    Menu leaderBoardMenu("kenvector_future", 32, 0, 100, 400, 700, false);
    std::string data = storage.getStringLeaderBoard();
    leaderBoardMenu.addNewItem(data, []() {});

    while (leaderBoardWindow.isOpen()) {
        sf::Event event;
        handleBasicEvents(event, leaderBoardWindow, controlMenu);
        leaderBoardWindow.clear();
        controlMenu.draw(&leaderBoardWindow);
        leaderBoardMenu.draw(&leaderBoardWindow);
        leaderBoardWindow.display();
    }
    leaderBoardMutex.unlock();
};

void credits() {
    creditsMutex.lock();
    sf::Thread mainMenuThread(&mainMenu);
    sf::RenderWindow creditsWindow(VIDEOMODE, TITLE + " - Credits");
    setupLogo(creditsWindow);
    Menu controlMenu("kenvector_future", 24, 500, 100, 400, 400, true);
    controlMenu.addNewItem("Back", [&]() { switchWindows(mainMenuMutex, creditsWindow, mainMenuThread); });
    controlMenu.addNewItem("Exit", [&]() { creditsWindow.close(); });
    Menu leaderBoardMenu("kenvector_future", 32, 0, 100, 400, 700, false);
    leaderBoardMenu.addNewItem("This game developed \nby Ahmed Eltaher\n& Mahmoud Nourelden \nRelease date : 1/5/2021 ", []() {});

    while (creditsWindow.isOpen()) {
        sf::Event event;
        handleBasicEvents(event, creditsWindow, controlMenu);
        creditsWindow.clear();
        controlMenu.draw(&creditsWindow);
        leaderBoardMenu.draw(&creditsWindow);
        creditsWindow.display();
    }
    creditsMutex.unlock();
};

void enterYourName() {
    enterYourNameMutex.lock();
    sf::Thread mainMenuThread(&mainMenu), choosePlayerShapeThread(&choosePlayerShape);
    sf::RenderWindow enterYourNameWindow(VIDEOMODE, TITLE + " - Enter Your Name");
    setupLogo(enterYourNameWindow);
    sf::Clock clock;
    sf::Time textEffectTime;
    bool showCursor = true;
    Menu controlMenu("kenvector_future", 24, 500, 100, 400, 400, true);
    controlMenu.addNewItem("Continue", [&]() { switchWindows(choosePlayerShapeMutex, enterYourNameWindow, choosePlayerShapeThread); });
    controlMenu.addNewItem("Back", [&]() { switchWindows(mainMenuMutex, enterYourNameWindow, mainMenuThread); });
    controlMenu.addNewItem("Exit", [&]() { enterYourNameWindow.close(); });
    Menu inputMenu("kenvector_future", 32, -20, 40, 400, 700, false);
    inputMenu.addNewItem("Enter Your Name (max 10 chars)", []() {});
    inputMenu.addNewItem(playerName.name, []() {});

    while (enterYourNameWindow.isOpen()) {
        sf::Event event;
        while (enterYourNameWindow.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                enterYourNameWindow.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::BackSpace) {
                    if (!playerName.name.empty()) {
                        playerName.name.pop_back();
                        playerName.length--;
                    }
                }
                if (std::isprint(event.text.unicode)) soundManager.playSFX("menu");
                controlMenu.handleEvents(event);
                break;
            case sf::Event::TextEntered:
                if (playerName.length < 10 && std::isprint(event.text.unicode)) {
                    playerName.name += event.text.unicode;
                    playerName.length++;
                }
                break;
            }
        }

        textEffectTime += clock.restart();

        if (textEffectTime >= sf::seconds(0.5f)) {
            showCursor = !showCursor;
            textEffectTime = sf::Time::Zero;
        }
        inputMenu.updateItem(1, playerName.name + (showCursor ? '_' : ' '));
        enterYourNameWindow.clear();
        controlMenu.draw(&enterYourNameWindow);
        inputMenu.draw(&enterYourNameWindow);
        enterYourNameWindow.display();
    }
    enterYourNameMutex.unlock();
};

void lose() {
    loseMutex.lock();
    sf::Thread mainMenuThread(&mainMenu);
    sf::RenderWindow loseWindow(VIDEOMODE, TITLE + " - You Lost");
    setupLogo(loseWindow);
    Menu controlMenu("kenvector_future", 24, 400, 100, 400, 400, true);
    controlMenu.addNewItem("Back to main Menu", [&]() { switchWindows(mainMenuMutex, loseWindow, mainMenuThread); });
    controlMenu.addNewItem("Exit", [&]() { loseWindow.close(); });
    Menu loseMenu("kenvector_future", 40, 0, 100, 400, 700, false);
    loseMenu.addNewItem("You Lost", []() {});

    while (loseWindow.isOpen()) {
        sf::Event event;
        handleBasicEvents(event, loseWindow, controlMenu);
        loseWindow.clear();
        controlMenu.draw(&loseWindow);
        loseMenu.draw(&loseWindow);
        loseWindow.display();
    }
    loseMutex.unlock();
};

void choosePlayerShape() {
    choosePlayerShapeMutex.lock();
    sf::Thread mainMenuThread(&mainMenu), choosePlayerColorThread(&choosePlayerColor);
    sf::RenderWindow choosePlayerShapeWindow(VIDEOMODE, TITLE + " - choose Player Shape");
    setupLogo(choosePlayerShapeWindow);
    sf::Clock clock;
    sf::Time textEffectTime;
    bool showCursor = true;
    Menu controlMenu("kenvector_future", 24, 500, 100, 400, 400, true);
    controlMenu.addNewItem("Continue", [&]() { switchWindows(choosePlayerColorMutex, choosePlayerShapeWindow, choosePlayerColorThread); });
    controlMenu.addNewItem("Back", [&]() { switchWindows(mainMenuMutex, choosePlayerShapeWindow, mainMenuThread); });
    controlMenu.addNewItem("Exit", [&]() { choosePlayerShapeWindow.close(); });
    Menu pickMenu("kenvector_future", 32, -20, 40, 400, 700, false);
    pickMenu.addNewItem("Choose Your Ship", []() {});
    std::vector <sf::Sprite> shapes;
    for (unsigned int i = 1; i <= 3; i++) {
        sf::Texture* shapeTexture = new sf::Texture;
        shapeTexture->loadFromFile("./textures/player/" + std::to_string(i) + ".png");
        sf::Sprite shape(*shapeTexture);
        shape.setPosition(100 * i + 50, 200);
        shape.setColor(sf::Color(128, 128, 128));
        shapes.push_back(shape);
    }
    while (choosePlayerShapeWindow.isOpen()) {
        sf::Event event;
        while (choosePlayerShapeWindow.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                controlMenu.handleEvents(event);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                if (selectedShape + 1 <= 3) {
                    selectedShape++;
                    for (unsigned int i = 0; i < 3; i++) {
                        shapes[i].setColor(sf::Color(128, 128, 128));
                    }
                    shapes[selectedShape - 1].setColor(sf::Color(255, 255, 255));
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                if (selectedShape - 1 > 0) {
                    selectedShape--;
                    for (unsigned int i = 0; i < 3; i++) {
                        shapes[i].setColor(sf::Color(128, 128, 128));
                    }
                    shapes[selectedShape - 1].setColor(sf::Color(255, 255, 255));
                }
            }
        }
            choosePlayerShapeWindow.clear();

            for (unsigned int i = 0; i < 3; i++) {
                choosePlayerShapeWindow.draw(shapes[i]);
            }
            controlMenu.draw(&choosePlayerShapeWindow);
            pickMenu.draw(&choosePlayerShapeWindow);
            choosePlayerShapeWindow.display();
        }
        choosePlayerShapeMutex.unlock();
};

void choosePlayerColor() {
    choosePlayerColorMutex.lock();
    sf::Thread mainMenuThread(&mainMenu), gameThread(&gameLoop);
    sf::RenderWindow choosePlayerColorWindow(VIDEOMODE, TITLE + " - choose Player Color");
    setupLogo(choosePlayerColorWindow);
    sf::Clock clock;
    sf::Time textEffectTime;
    bool showCursor = true;
    Menu controlMenu("kenvector_future", 24, 500, 100, 400, 400, true);
    controlMenu.addNewItem("Continue", [&]() { switchWindows(gameMutex, choosePlayerColorWindow, gameThread); });
    controlMenu.addNewItem("Back", [&]() { switchWindows(mainMenuMutex, choosePlayerColorWindow, mainMenuThread); });
    controlMenu.addNewItem("Exit", [&]() { choosePlayerColorWindow.close(); });
    Menu pickMenu("kenvector_future", 32, -20, 40, 400, 700, false);
    pickMenu.addNewItem("Choose Your Ship", []() {});
    std::vector <sf::Sprite> shapes;
    for (unsigned int i = 0; i < 4; i++) {
        sf::Texture* shapeTexture = new sf::Texture;
        shapeTexture->loadFromFile("./textures/player/" + std::to_string(selectedShape) + "/" + playerColors[i] + ".png");
        sf::Sprite shape(*shapeTexture);
        shape.setPosition(100 * i + 50, 200);
        shape.setColor(sf::Color(128, 128, 128));
        shapes.push_back(shape);
    }
    while (choosePlayerColorWindow.isOpen()) {
        sf::Event event;
        while (choosePlayerColorWindow.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                controlMenu.handleEvents(event);
                sf::Texture* playerTexture = new sf::Texture;
                playerTexture->loadFromFile("./textures/player/" + std::to_string(selectedShape) + "/" + playerColors[selectedColor] + ".png");
                player.sprite.setTexture(*playerTexture);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                if (selectedColor + 1 < 4) {
                    selectedColor++;
                    for (unsigned int i = 0; i < 4; i++) {
                        shapes[i].setColor(sf::Color(128, 128, 128));
                    }
                    shapes[selectedColor].setColor(sf::Color(255, 255, 255));
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                if (selectedColor - 1 >= 0) {
                    selectedColor--;
                    for (unsigned int i = 0; i < 4; i++) {
                        shapes[i].setColor(sf::Color(128, 128, 128));
                    }
                    shapes[selectedColor].setColor(sf::Color(255, 255, 255));
                }
            }
        }
        choosePlayerColorWindow.clear();

        for (unsigned int i = 0; i < 4; i++) {
            choosePlayerColorWindow.draw(shapes[i]);
        }
        controlMenu.draw(&choosePlayerColorWindow);
        pickMenu.draw(&choosePlayerColorWindow);
        choosePlayerColorWindow.display();
    }
    choosePlayerColorMutex.unlock();
};


void gameLoop() {
    gameMutex.lock();
    init();
    sf::Thread mainMenuThread(&mainMenu),  loseThread(&lose);
    sf::RenderWindow gameWindow(VIDEOMODE, TITLE);
    gameWindow.setMouseCursorVisible(false); // To Hide The Default Cursor
    gameWindow.setKeyRepeatEnabled(false); // To Register The Keyboard input only once at frame
    setupLogo(gameWindow);
    bool gamePaused = false, whiteSpacesOnly = std::all_of(playerName.name.begin(), playerName.name.end(), isspace);
    FPS fps;
    sf::Clock clock;
    // Start: Files To Load Before Run
    loadBulletLevels("bulletLevels");
    loadMeteors("meteors");
    // End: Files To Load Before Run
        // Start: Intervals
    setInterval(updateDifficulty, 60000); // Change Difficulty dipending on Score
    setInterval(lucky, 10000); // Every 10 Seconds It might Spawn a Lucky Meteor
    setInterval(newEnemy, 2500);
    // End: Intervals

    playerName.name = (playerName.name == "" || whiteSpacesOnly) ? "Unknow" : playerName.name;
    Menu leftMenu("kenvector_future", 18, 0, 25, 200, 100, false);
    leftMenu.addNewItem("Player: " + playerName.name, []() {});
    leftMenu.addNewItem("FPS: 0", []() {});
    leftMenu.addNewItem("Score: 0", []() {});
    leftMenu.addNewItem("Lives: 0", []() {});

    Menu rightMenu("kenvector_future", 18, 500, 25, 200, 100, false);
    rightMenu.addNewItem("Diffculty: 0", []() {});
    rightMenu.addNewItem("Time: 0", []() {});

    Menu pauseMenu("kenvector_future", 40, 200, 100, 100, 400, true);
    pauseMenu.addNewItem("Resume", [&]() { gamePaused = false; });
    pauseMenu.addNewItem("Back To Main Menu", [&]() { switchWindows(mainMenuMutex, gameWindow, mainMenuThread); });
    pauseMenu.addNewItem("Exit", [&]() { gameWindow.close(); });
    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                gameWindow.close();
                break;
            case sf::Event::LostFocus:
                gamePaused = true;
                break;
            case sf::Event::GainedFocus:
                gamePaused = false;
                break;
            case sf::Event::KeyPressed:
                if (gamePaused) {
                    soundManager.playSFX("menu");
                    pauseMenu.handleEvents(event);
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    if (gamePaused) 
                        gamePaused = false;
                    else 
                        gamePaused = true;
                }
                break;
            }
        }
        cursor.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(gameWindow)));
        if (cursor.getPosition().x < 0) cursor.setPosition(0, cursor.getPosition().y);
        if (cursor.getPosition().y < 0) cursor.setPosition(cursor.getPosition().x, 0);
        if (cursor.getPosition().x > APP_SIZE.x) cursor.setPosition(APP_SIZE.x, cursor.getPosition().y);
        if (cursor.getPosition().y > APP_SIZE.y) cursor.setPosition(cursor.getPosition().x, APP_SIZE.y);
        leftMenu.updateItem(1, "FPS: " + std::to_string(fps.getFPS()));
        leftMenu.updateItem(2, "Score: " + std::to_string(player.score));
        leftMenu.updateItem(3, "Lives: " + std::to_string(player.lives));
        rightMenu.updateItem(0, "Diffculty: " + std::to_string(difficultyLevel));
        rightMenu.updateItem(1, "Time: " + std::to_string((int)totalTime.getElapsedTime().asSeconds()));
        gameWindow.clear();
        if (!gamePaused) {
            draw(&gameWindow);
            handleAllEnemies(&gameWindow);
            player.update();
            animateBackground();
            leftMenu.draw(&gameWindow);
            rightMenu.draw(&gameWindow);
            if (player.lives < 0) {
                soundManager.playSFX("Lost_All_life");
                switchWindows(loseMutex, gameWindow, loseThread);
                soundManager.pause();
                storage.addData(playerName.name + " " + std::to_string(player.score));
            }
            //gameWindow.clear();
            fps.update(); // Call fps to be updated every frame otherwise it will always be 0
            deltaTime = clock.restart().asSeconds();
        }
        else pauseMenu.draw(&gameWindow);
        gameWindow.display();
    }
    gameMutex.unlock();
};

int main() {
    mainMenu();
    return EXIT_SUCCESS;
};

void init() {
    // Start: Font
    font->loadFromFile("./fonts/kenvector_future.ttf");
    // End: Font

    // Start: Background
    backgroundTexture.setRepeated(true);
    backgroundTexture.loadFromFile("./textures/background.png");
    background.setTextureRect(sf::IntRect(0, 0, 800, 600));
    // End: Background

    // Start: Cursor
    sf::Texture* cursorTexture = new sf::Texture;
    cursorTexture->loadFromFile("./textures/cursor.png");
    cursor.setTexture(*cursorTexture);
    cursor.setOrigin((float)cursorTexture->getSize().x / 2.f, (float)cursorTexture->getSize().y / 2.f);
    // End: Cursor

    settings.antialiasingLevel = 8;
};

void loadBulletLevels(std::string fileName) {
    std::ifstream loadBulletLevelsFile("./loaders/" + fileName + ".txt");
    if(loadBulletLevelsFile.is_open()) {
        std::string line, textureFile;
        int speed, damage;
        while (std::getline(loadBulletLevelsFile, line)) {
            std::istringstream in(line);
            in >> textureFile >> speed >> damage;
            bulletLevels.push_back({ textureFile, speed, damage });
            maxBulletTypesCount++;
        }
    }
    loadBulletLevelsFile.close();
};

void loadMeteors(std::string fileName) {
    std::ifstream loadMeteorsFile("./loaders/" + fileName + ".txt");
    if (loadMeteorsFile.is_open()) {
        std::string line, size;
        int index;
        while (std::getline(loadMeteorsFile, line)) {
            std::istringstream in(line);
            in >> size >> index;
            for (int i = 0; i < 2; i++) {
                meteorTypes[colors[i]][size].push_back(index);
            }
        }
    }
    loadMeteorsFile.close();
};

void lucky() {
    canSpawnLuckyMeteor = (std::rand() % 8 == 0);
    canSpawnLuckyUFO = (std::rand() % 6 == 0);
};

sf::Vector2f findPath(sf::Vector2f playerPosition, sf::Sprite sprite) {
    float x = 0, y = 0;
    float d = sqrt((playerPosition.x - sprite.getPosition().x) * (playerPosition.x - sprite.getPosition().x) - (playerPosition.y - sprite.getPosition().y) * (playerPosition.y - sprite.getPosition().y));
    y = (playerPosition.y - sprite.getPosition().y) / d;
    x = (playerPosition.x - sprite.getPosition().x) / d;
    return sf::Vector2f{ x, y };
};

bool isInScreenBounds(sf::Sprite sprite) {
    return (sprite.getPosition().x < APP_SIZE.x) && (sprite.getPosition().x + sprite.getGlobalBounds().width > 0) && 
            (sprite.getPosition().y < APP_SIZE.y) && (sprite.getPosition().y + sprite.getGlobalBounds().height > 0);
};

void spawnMeteors() {
    srand((unsigned)time(0));
    int meteorsCount = 8 + difficultyLevel;
    for (int i = 0; i < meteorsCount; i++) {
        int sizeIndex = (std::rand() % (sizeof(sizes) / sizeof(sizes[0])));
        std::string color = colors[(std::rand() % (sizeof(colors) / sizeof(colors[0])))];
        std::string size = sizes[sizeIndex];
        int index = (std::rand() % (meteorTypes[color][size].size())) + 1;
        sf::Sprite* meteorSprite = new sf::Sprite;
        meteorSprite->setPosition((std::rand() % 200) + APP_SIZE.x + 200 * i, std::rand() % (int)APP_SIZE.y);
        sf::Texture* meteorTexture = new sf::Texture;
        bool isGolden;
        int framesCount;
        if (canSpawnLuckyMeteor) {
            meteorTexture->loadFromFile("./textures/meteors/golden.png");
            canSpawnLuckyMeteor = false;
            sizeIndex = -1;
            isGolden = true;
            framesCount = 11;
        }
        else {
            meteorTexture->loadFromFile("./textures/meteors/" + color + "/" + size + "-" + std::to_string(index) + ".png");
            isGolden = false;
            framesCount = 0;
        }
        meteorSprite->setTexture(*meteorTexture);
        int speed = 220 - (50 * sizeIndex);
        int scoreToGain = 15 - (5 * sizeIndex);
        bool isDividable = ((size == "big") ? true: false);
        meteors.push_back({ meteorSprite, {}, size, color, speed, scoreToGain, isDividable, false, isGolden, framesCount, 0 });
    }
};

void handleAllEnemies(sf::RenderWindow* window) {
    if (canSpawnLuckyUFO) {
        UnidentifiedFlyingObject ufo;
        UFOs.push_back(ufo);
        canSpawnLuckyUFO = false;
    }
    if (UFOs.size() > 0) {
        for (int i = 0; i < UFOs.size(); i++) {
            if (!UFOs[i].isDone) {
                UFOs[i].move();
                UFOs[i].drawBullets(window);
                for (unsigned int j = 0; j < UFOs[i].bullets.size(); j++) {
                    if (UFOs[i].bullets[j].getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
                        player.takeDamge(UFOs[i].bulletDamage);
                        player.decreasePowerLevel();
                        UFOs[i].bullets.erase(UFOs[i].bullets.begin() + j);
                        for (unsigned int k = 0; k < enemies.size(); k++)
                            enemies[k].bullets.clear();
                        break;
                    }
                }
                UFOs[i].draw(window);
                for (unsigned int j = 0; j < player.bullets.size(); j++) {
                    if (UFOs[i].sprite.getGlobalBounds().intersects(player.bullets[j].sprite.getGlobalBounds())) {
                        newTextEffect(50, TextEffectTypes::INCREASE, UFOs[i].sprite.getPosition());
                        player.score += 50;
                        player.increasePowerLevel();
                        UFOs.erase(UFOs.begin() + i);
                        break;
                    }
                }
            }
            else
                UFOs.erase(UFOs.begin() + i);
        }
    }
    if (meteors.size() == 0)
        spawnMeteors();
    handleEnemy(window);
};


void animateBackground() {
    if (abs(animateTimer.getElapsedTime().asSeconds() - lastAnimateTime) >= 0.05) {
        backgroundAnimationCounter += 2;
        background.setTextureRect(sf::IntRect(backgroundAnimationCounter, backgroundAnimationCounter, 800, 600));
        lastAnimateTime = animateTimer.restart().asSeconds();
    }
};


void updateDifficulty() {
    difficultyLevel = (player.score % 1000) ? player.score / 1000 + 1 : player.score / 1000;
};

void newTextEffect(int score, TextEffectTypes type, sf::Vector2f startPosition) {
    sf::Text text;
    text.setFont(*font);
    text.setString(((type == TextEffectTypes::INCREASE) ? "+" : "-") + std::to_string(score));
    text.setFillColor(((type == TextEffectTypes::INCREASE) ? sf::Color(50, 205, 50, 255) : sf::Color(255, 0, 56, 255)));
    text.setPosition(startPosition);
    text.setCharacterSize(24);
    addTextEffect(text, startPosition.y - 100);
};

void newFloatingText(std::string floatingText, TextEffectTypes type, sf::Vector2f startPosition) {
    sf::Text text;
    text.setFont(*font);
    text.setString(floatingText);
    text.setFillColor(((type == TextEffectTypes::INCREASE) ? sf::Color(50, 205, 50, 255) : sf::Color(255, 0, 56, 255)));
    text.setPosition(startPosition);
    text.setCharacterSize(24);
    addTextEffect(text, startPosition.y - 100);
};


void addTextEffect(sf::Text text, float maxPosition) {
    textEffects.push_back({ text, maxPosition, text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, (float)text.getFillColor().a });
}

void draw(sf::RenderWindow* window) {
    window->draw(background); // Rendring Background
    player.draw(window);
    if (textEffects.size() > 0) drawTextEffects(window); // Rendreing TextEffects if needed
    if (meteors.size() > 0) drawMeteors(window); // Rendreing Meteors if exist
    window->draw(cursor); // Rendring Cursor
};

void drawMeteors(sf::RenderWindow* window) {
    for (unsigned int i = 0; i < meteors.size(); i++) {
        if (meteors[i].isPathFound) {
            meteors[i].sprite->move(meteors[i].linearPath.x * meteors[i].speed * deltaTime, meteors[i].linearPath.y * meteors[i].speed * deltaTime);
            if (isInScreenBounds(*meteors[i].sprite)) window->draw(*meteors[i].sprite); // Will be Draw only if the  Meteors is in the screen bound
            meteors[i].sprite->rotate(0.01);
            if (meteors[i].isGolden) meteors[i].animate();
            if (meteors[i].sprite->getPosition().x < 0 || meteors[i].sprite->getPosition().y < 0) {
                if (player.score - meteors[i].scoreToGain >= 0) {
                    player.score -= meteors[i].scoreToGain;
                    newTextEffect(5, TextEffectTypes::DECREASE, { 0, meteors[i].sprite->getPosition().y });
                }
                meteors[i].sprite->setPosition((std::rand() % 200) + (int)APP_SIZE.x + 100 * i, std::rand() % (int)APP_SIZE.y);
                meteors[i].linearPath = findPath(player.sprite.getPosition(), *meteors[i].sprite);
            }
            for (unsigned int j = 0; j < player.bullets.size(); j++) {
                if (meteors[i].sprite->getGlobalBounds().intersects(player.bullets[j].sprite.getGlobalBounds())) {
                    soundManager.playSFX(meteors[i].size + "_Stone");
                    player.bullets.erase(player.bullets.begin() + j);
                    newTextEffect(meteors[i].scoreToGain, TextEffectTypes::INCREASE, meteors[i].sprite->getPosition());
                    player.score += meteors[i].scoreToGain;
                    if (meteors[i].isDividable) {
                        for (int k = 0; k < 2; k++) {
                            int sizeIndex = (std::rand() % ((sizeof(sizes) / sizeof(sizes[0])) - 1));
                            std::string color = meteors[i].color;
                            std::string size = sizes[sizeIndex];
                            int index = (std::rand() % (meteorTypes[color][size].size())) + 1;
                            sf::Sprite* meteorSprite = new sf::Sprite;
                            meteorSprite->setPosition(meteors[i].sprite->getPosition().x + 50 * (k + 1), meteors[i].sprite->getPosition().y - 50 * (k + 1));
                            sf::Texture* meteorTexture = new sf::Texture;
                            meteorTexture->loadFromFile("./textures/meteors/" + color + "/" + size + "-" + std::to_string(index) + ".png");
                            meteorSprite->setTexture(*meteorTexture);
                            int speed = 220 - (50 * sizeIndex);
                            int scoreToGain = 15 - (5 * sizeIndex);
                            meteors.push_back({ meteorSprite, {}, size, color, speed, scoreToGain, false, false, false, 0, 0 });
                        }
                    }
                    meteors.erase(meteors.begin() + i);
                    break;
                }
            }
        }
        else {
            meteors[i].linearPath = findPath(player.sprite.getPosition(), *meteors[i].sprite);
            meteors[i].isPathFound = true;
        }
    } // Meteors Logic and Drawign
};
void handleEnemy(sf::RenderWindow* window) {
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].draw(window);
        enemies[i].TakePlace();
        enemies[i].rotate();
        enemies[i].shoot();
        if (!enemies[i].bullets.empty()) {
            for (unsigned int j = 0; j < enemies[i].bullets.size(); j++) {
                enemies[i].bullets[j].move(-(float)sin(enemies[i].bullets[j].getRotation() * PI / 180) * enemies[i].bulletSpeed * deltaTime, (float)cos(enemies[i].bullets[j].getRotation() * PI / 180) * enemies[i].bulletSpeed * deltaTime);
                window->draw(enemies[i].bullets[j]);
                if (enemies[i].bullets[j].getGlobalBounds().intersects(player.sprite.getGlobalBounds())) {
                    player.takeDamge(enemies[i].bulletDamage);
                    player.decreasePowerLevel();
                    enemies[i].bullets.erase(enemies[i].bullets.begin() + j);
                    for (unsigned int k = 0; k < enemies.size(); k++)
                        enemies[k].bullets.clear();
                    break;
                }
            }
        }
        for (unsigned int j = 0; j < player.bullets.size(); j++) {
            if (enemies[i].sprite.getGlobalBounds().intersects(player.bullets[j].sprite.getGlobalBounds())) {
                soundManager.playSFX("Enemy_Ship_Destroyed");
                newTextEffect(35, TextEffectTypes::INCREASE, enemies[i].sprite.getPosition());
                player.score += 35;
                player.bullets.erase(player.bullets.begin() + j);
                enemies.erase(enemies.begin() + i);
                break;
            }
        }
        
    }
}

void drawTextEffects(sf::RenderWindow* window) {
    for (unsigned int i = 0; i < textEffects.size(); i++) {
        // Will be Draw only if the TextEffect is in the Moving Range
        if (textEffects[i].text.getPosition().y > textEffects[i].maxPosition) {
            textEffects[i].text.move(0, -100 * deltaTime);
            textEffects[i].opacity -= (float)((textEffects[i].opacity > 0) ? 0.1 : 0);
            textEffects[i].text.setFillColor(sf::Color(textEffects[i].red, textEffects[i].green, textEffects[i].blue, (sf::Uint8)textEffects[i].opacity));
            window->draw(textEffects[i].text);
        }
        else // if it went out the Range it will be delete
            textEffects.erase(textEffects.begin() + i);
    } // Score Text Effect Logic and Drawign
};
