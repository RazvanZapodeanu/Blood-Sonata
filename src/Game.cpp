#include "../include/Game.h"
#include <iostream>
#include <stdexcept>

Game& Game::getInstance() {
    static Game instance;
    return instance;
}

Game::Game()
    : m_gameState(GameState::MENU)
    , m_enemyPool(10)
    , m_gameOverTimer(0.0f)
    , m_debugMode(false)
    , m_backgroundSprite(m_backgroundTexture)
{
}

Game::~Game() {

}

void Game::initialize() {

    initializeWindow();


    initializeGame();


    setGameState(GameState::MENU);
}

void Game::initializeWindow() {

    m_window.create(sf::VideoMode(sf::Vector2u(1280, 720)), "Blood Sonata", sf::Style::Close | sf::Style::Titlebar);
    m_window.setFramerateLimit(60);


    sf::Image icon;
    if (icon.loadFromFile("assets/icon.jpg")) {
        m_window.setIcon(sf::Vector2u(icon.getSize().x, icon.getSize().y), icon.getPixelsPtr());
    }
}

void Game::initializeGame() {
    try {

        auto& resourceManager = ResourceManager::getInstance();
        resourceManager.loadTexture("background", "assets/map.jpg");
        resourceManager.loadFont("main_font", "assets/fonts/main_font.otf");


        auto& soundManager = SoundManager::getInstance();
        soundManager.loadSound("punch", "assets/sounds/punch.wav");
        soundManager.loadSound("kick", "assets/sounds/kick.wav");
        soundManager.loadSound("jump", "assets/sounds/jump.wav");
        soundManager.loadSound("land", "assets/sounds/land.wav");
        soundManager.loadSound("punch_hit", "assets/sounds/punch_hit.wav");
        soundManager.loadSound("kick_hit", "assets/sounds/kick_hit.wav");
        soundManager.loadSound("parry", "assets/sounds/parry.wav");
        soundManager.loadSound("parry_start", "assets/sounds/parry_start.wav");
        soundManager.loadSound("death", "assets/sounds/death.wav");
        soundManager.loadSound("enemy_attack", "assets/sounds/enemy_attack.wav");
        soundManager.loadSound("enemy_spawn", "assets/sounds/enemy_spawn.wav");
        soundManager.loadSound("button_click", "assets/sounds/button_click.wav");

        soundManager.loadMusic("menu", "assets/sounds/menu_music.ogg");
        soundManager.loadMusic("game", "assets/sounds/game_music.ogg");
        soundManager.loadMusic("game_over", "assets/sounds/game_over_music.ogg");


        auto& effectManager = EffectManager::getInstance();
        effectManager.loadEffect("punch", "assets/sprites/effects/punch_effect.png", 0.3f);
        effectManager.loadEffect("kick", "assets/sprites/effects/kick_effect.png", 0.3f);
        effectManager.loadEffect("air_attack", "assets/sprites/effects/air_attack_effect.png", 0.3f);
        effectManager.loadEffect("enemy_attack", "assets/sprites/effects/enemy_attack_effect.png", 0.3f);
        effectManager.loadEffect("blood", "assets/sprites/effects/blood_effect.png", 0.5f);
        effectManager.loadEffect("parry", "assets/sprites/effects/parry_effect.png", 0.3f);
        effectManager.loadEffect("spawn", "assets/sprites/effects/spawn_effect.png", 0.5f);


        m_backgroundTexture = resourceManager.getTexture("background");
        m_backgroundSprite = sf::Sprite(m_backgroundTexture);  // Creează sprite nou cu textura
        m_backgroundSprite.setPosition(sf::Vector2f(0.0f, 0.0f));


        m_player = std::make_unique<Player>(640.0f, 300.0f);


        m_spawner = std::make_unique<Spawner>(m_enemyPool, m_player.get());
        m_spawner->setSpawnArea(100.0f, 1180.0f, 300.0f);


        m_camera = std::make_unique<Camera>(sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1280.0f, 720.0f)));
        m_camera->setTarget(m_player.get());


        m_uiManager = std::make_unique<UIManager>(m_window);
        m_uiManager->initialize();

    } catch (const std::exception& e) {
        throw std::runtime_error("Error during game startup: " + std::string(e.what()));
    }
}

void Game::run() {

    SoundManager::getInstance().playMusic("menu");


    while (m_window.isOpen()) {

        float deltaTime = m_clock.restart().asSeconds();


        processEvents();


        update(deltaTime);


        render();
    }
}

void Game::processEvents() {
    while (auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            const auto key = keyPressed->scancode;
            std::cout << "KEY PRESSED EVENT: " << static_cast<int>(key) << std::endl;

            if (key == sf::Keyboard::Scancode::Escape) {
                if (m_gameState == GameState::PLAYING) {
                    setGameState(GameState::PAUSED);
                } else if (m_gameState == GameState::PAUSED) {
                    setGameState(GameState::PLAYING);
                }
            }
            else if (key == sf::Keyboard::Scancode::S && m_gameState == GameState::PLAYING) {
                spawnEnemy();
            }
            else if (key == sf::Keyboard::Scancode::F1) {
                m_debugMode = !m_debugMode;
            }
        }


        m_uiManager->handleInput(*event);


        InputManager::getInstance().processEvent(*event);
    }


}

void Game::update(float deltaTime) {

    InputManager::getInstance().update();


    switch (m_gameState) {
        case GameState::MENU:
            m_uiManager->update(deltaTime);
        break;

        case GameState::PLAYING:

                m_player->update(deltaTime);


        for (auto& enemy : m_enemies) {
            enemy->update(deltaTime);
        }


        checkCollisions();


        m_camera->update(deltaTime);
        EffectManager::getInstance().update(deltaTime);
        EffectManager::getInstance().cleanupFinishedEffects();


        if (!m_player->isAlive()) {
            m_gameOverTimer += deltaTime;
            if (m_gameOverTimer >= GAME_OVER_DELAY) {
                setGameState(GameState::GAME_OVER);
            }
        }
        break;

        case GameState::PAUSED:
            m_uiManager->update(deltaTime);
        break;

        case GameState::GAME_OVER:
            m_uiManager->update(deltaTime);
        break;
    }
}


void Game::render() {

    m_window.clear(sf::Color(102, 47, 47));


    switch (m_gameState) {
        case GameState::MENU:

            m_uiManager->render();
            break;

        case GameState::PLAYING:
        case GameState::PAUSED:



            m_window.setView(m_camera->getView());


            m_window.draw(m_backgroundSprite);


            EffectManager::getInstance().render(m_window);

            for (auto& enemy : m_enemies) {
                enemy->render(m_window);
            }


            m_player->render(m_window);


            m_window.setView(m_window.getDefaultView());


            if (m_gameState == GameState::PAUSED) {
                m_uiManager->render();
            }

            break;

        case GameState::GAME_OVER:



            m_window.setView(m_camera->getView());


            m_window.draw(m_backgroundSprite);


            EffectManager::getInstance().render(m_window);


            for (auto& enemy : m_enemies) {
                enemy->render(m_window);
            }


            m_player->render(m_window);


            m_window.setView(m_window.getDefaultView());


            m_uiManager->render();

            break;
    }


    m_window.display();
}

void Game::checkCollisions() {

    static bool wasJPressed = false;
    static bool wasKPressed = false;
    static bool wasLPressed = false;

    bool jPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::J);
    bool kPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::K);
    bool lPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::L);

    for (auto it = m_enemies.begin(); it != m_enemies.end();) {
        auto& enemy = *it;

        bool inAttackRange = m_player->checkAttackRange(dynamic_cast<const Entity&>(*enemy));

        if (inAttackRange) {
            bool justAttacked = false;
            float damage = 0.0f;
            std::string attackType = "";


            if (jPressed && !wasJPressed) {
                damage = 20.0f;
                attackType = "punch";
                justAttacked = true;
                std::cout << "=== PUNCH HIT! ===" << std::endl;
            }
            else if (kPressed && !wasKPressed) {
                damage = 30.0f;
                attackType = "kick";
                justAttacked = true;
                std::cout << "=== KICK HIT! ===" << std::endl;
            }
            else if (lPressed && !wasLPressed && m_player->isJumping()) {
                damage = 40.0f;
                attackType = "air_attack";
                justAttacked = true;
                std::cout << "=== AIR ATTACK HIT! ===" << std::endl;
            }

            if (justAttacked) {
                enemy->takeDamage(damage, attackType);

                if (!enemy->isAlive()) {
                    std::cout << "=== ENEMY KILLED! ===" << std::endl;
                    it = m_enemies.erase(it);

                    wasJPressed = jPressed;
                    wasKPressed = kPressed;
                    wasLPressed = lPressed;
                    continue;
                }
            }
        }


        bool inPhysicalRange = m_player->checkCollision(dynamic_cast<const Entity&>(*enemy));

        if (inPhysicalRange) {

            if (enemy->canAttack()) {
                if (!m_player->isParrying()) {
                    std::cout << "ENEMY HIT PLAYER!" << std::endl;
                    m_player->takeDamage(5.0f);
                }
            }


            sf::Vector2f playerPos = m_player->getPosition();
            sf::Vector2f enemyPos = enemy->getPosition();
            sf::Vector2f direction = playerPos - enemyPos;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) {
                direction /= length;
                m_player->move(direction.x * 3.0f, 0.0f);
                enemy->move(-direction.x * 3.0f, 0.0f);
            }
        }

        ++it;
    }


    wasJPressed = jPressed;
    wasKPressed = kPressed;
    wasLPressed = lPressed;
}



void Game::spawnEnemy() {

    auto enemy = m_spawner->spawnEnemy();


    m_enemies.push_back(std::move(enemy));
}

void Game::reset() {

    m_player = std::make_unique<Player>(640.0f, 300.0f);


    m_enemies.clear();


    m_camera->setTarget(m_player.get());


    m_gameOverTimer = 0.0f;


    EffectManager::getInstance().clearAllEffects();


    m_spawner->setTarget(m_player.get());


    setGameState(GameState::PLAYING);


    SoundManager::getInstance().stopAllMusic();
    SoundManager::getInstance().playMusic("game");
}

void Game::exit() {

    m_window.close();
}

void Game::setGameState(GameState state) {

    GameState previousState = m_gameState;


    m_gameState = state;


    if (previousState != m_gameState) {
        switch (m_gameState) {
            case GameState::MENU:

                m_uiManager->setActiveMenu(MenuType::MAIN);


                SoundManager::getInstance().stopAllMusic();
                SoundManager::getInstance().playMusic("menu");
                break;

            case GameState::PLAYING:

                m_uiManager->setActiveMenu(MenuType::MAIN);
                m_uiManager->isMenuActive(MenuType::MAIN) ? m_uiManager->setActiveMenu(MenuType::MAIN) : void();


                if (previousState == GameState::MENU || previousState == GameState::GAME_OVER) {
                    SoundManager::getInstance().stopAllMusic();
                    SoundManager::getInstance().playMusic("game");
                }
                break;

            case GameState::PAUSED:

                m_uiManager->setActiveMenu(MenuType::PAUSE);
                break;

            case GameState::GAME_OVER:

                m_uiManager->setActiveMenu(MenuType::GAME_OVER);


                SoundManager::getInstance().stopAllMusic();
                SoundManager::getInstance().playMusic("game_over");
                break;
        }
    }
}

GameState Game::getGameState() const {
    return m_gameState;
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "Game: state=";

    switch (game.m_gameState) {
        case GameState::MENU:
            os << "MENU";
            break;
        case GameState::PLAYING:
            os << "PLAYING";
            break;
        case GameState::PAUSED:
            os << "PAUSED";
            break;
        case GameState::GAME_OVER:
            os << "GAME_OVER";
            break;
    }

    os << ", enemies=" << game.m_enemies.size();

    return os;
}