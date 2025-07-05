#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "Camera.h"
#include "UIManager.h"
#include "Spawner.h"
#include "templates/Pool.h"


enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
};


class Game {
public:

    static Game& getInstance();


    void initialize();


    void run();


    void reset();


    void exit();


    void spawnEnemy();


    void setGameState(GameState state);
    GameState getGameState() const;


    friend std::ostream& operator<<(std::ostream& os, const Game& game);

private:

    Game();


    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;


    ~Game();


    void processEvents();
    void update(float deltaTime);
    void render();


    void checkCollisions();


    void checkAttacks();


    void initializeWindow();
    void initializeGame();


    sf::RenderWindow m_window;


    sf::Clock m_clock;


    GameState m_gameState;


    std::unique_ptr<Player> m_player;


    std::vector<std::unique_ptr<Enemy>> m_enemies;


    Pool<Enemy> m_enemyPool;


    std::unique_ptr<Spawner> m_spawner;


    std::unique_ptr<Camera> m_camera;


    std::unique_ptr<UIManager> m_uiManager;


    sf::Sprite m_backgroundSprite;
    sf::Texture m_backgroundTexture;


    float m_gameOverTimer;
    static constexpr float GAME_OVER_DELAY = 3.0f;


    bool m_debugMode;
};

#endif