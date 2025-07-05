#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include <memory>
#include <random>


class Enemy : public Entity {
public:

    Enemy(float posX, float posY, float health = 100.0f);
    

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void attack() override;
    

    std::unique_ptr<Entity> clone() const override;
    

    void setTarget(Entity* target);
    

    bool canAttack() const;
    

    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy);

protected:

    virtual void initializeAnimations();
    

    virtual void updateAI(float deltaTime);
    

    bool shouldMove() const;
    bool shouldAttack() const;
    bool shouldParry() const;
    

    Entity* m_target;
    

    static constexpr float MOVE_SPEED = 200.0f;
    static constexpr float ATTACK_DAMAGE = 15.0f;
    static constexpr float ATTACK_RANGE = 120.0f;
    

    float m_attackCooldown;
    float m_attackTimer;
    

    float m_parryCooldown;
    float m_parryTimer;
    

    float m_decisionTimer;
    float m_decisionInterval;
    

    static std::mt19937 s_randomEngine;
    

    void updateAnimation();
};

#endif