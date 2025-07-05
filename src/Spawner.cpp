#include "../include/Spawner.h"
#include "../include/SoundManager.h"
#include "../include/EffectManager.h"
#include <iostream>
#include <ctime>

Spawner::Spawner(Pool<Enemy>& enemyPool, Entity* target)
    : m_enemyPool(enemyPool)
    , m_target(target)
    , m_minX(100.0f)
    , m_maxX(900.0f)
    , m_spawnY(0.0f)
    , m_randomEngine(static_cast<unsigned long>(std::time(nullptr)))
{

}

Spawner::~Spawner() {

}

std::unique_ptr<Enemy> Spawner::spawnEnemy() {

    std::unique_ptr<Enemy> enemy = m_enemyPool.acquire();


    sf::Vector2f spawnPos = getRandomSpawnPosition();


    if (!enemy) {
        enemy = std::make_unique<Enemy>(spawnPos.x, spawnPos.y);
    } else {

        enemy->setPosition(spawnPos.x, spawnPos.y);
    }
    

    enemy->setTarget(m_target);
    

    SoundManager::getInstance().playSound("enemy_spawn");
    

    auto pos = enemy->getPosition();
    EffectManager::getInstance().createEffect("spawn", sf::Vector2f(pos.x,pos.y+300));
    
    return enemy;
}

void Spawner::addEnemyType(const std::string& texturePath) {
    m_enemyTypes.push_back(texturePath);
}

void Spawner::setSpawnPosition(const sf::Vector2f& position) {
    m_minX = position.x;
    m_maxX = position.x;
    m_spawnY = position.y;
}

void Spawner::setSpawnArea(float minX, float maxX, float y) {
    m_minX = minX;
    m_maxX = maxX;
    m_spawnY = y;
}

void Spawner::setTarget(Entity* target) {
    m_target = target;
}

const std::string& Spawner::getRandomEnemyType() const {
    if (m_enemyTypes.empty()) {
        throw std::runtime_error("Spawner has no defined enemy types.");
    }
    
    std::uniform_int_distribution<size_t> dist(0, m_enemyTypes.size() - 1);
    return m_enemyTypes[dist(m_randomEngine)];
}

sf::Vector2f Spawner::getRandomSpawnPosition() const {
    std::uniform_real_distribution<float> distX(m_minX, m_maxX);
    return sf::Vector2f(distX(m_randomEngine), m_spawnY);
}

std::ostream& operator<<(std::ostream& os, const Spawner& spawner) {
    os << "Spawner: enemy types=" << spawner.m_enemyTypes.size()
       << ", spawn area=(" << spawner.m_minX << "-" << spawner.m_maxX << ", " << spawner.m_spawnY << ")";
    
    return os;
}