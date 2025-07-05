#ifndef SPAWNER_H
#define SPAWNER_H

#include <memory>
#include <vector>
#include <string>
#include <random>
#include "Entity.h"
#include "Enemy.h"
#include "templates/Pool.h"


class Spawner {
public:

    Spawner(Pool<Enemy>& enemyPool, Entity* target);
    

    ~Spawner();
    

    std::unique_ptr<Enemy> spawnEnemy();
    

    void addEnemyType(const std::string& texturePath);
    

    void setSpawnPosition(const sf::Vector2f& position);
    

    void setSpawnArea(float minX, float maxX, float y);
    

    void setTarget(Entity* target);
    

    friend std::ostream& operator<<(std::ostream& os, const Spawner& spawner);

private:

    std::vector<std::string> m_enemyTypes;
    

    Pool<Enemy>& m_enemyPool;
    

    Entity* m_target;
    

    float m_minX;
    float m_maxX;
    float m_spawnY;
    

    mutable std::mt19937 m_randomEngine;
    

    const std::string& getRandomEnemyType() const;
    

    sf::Vector2f getRandomSpawnPosition() const;
};

#endif