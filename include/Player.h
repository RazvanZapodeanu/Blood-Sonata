#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "InputManager.h"
#include <memory>


class Player : public Entity {
public:

    Player(float posX, float posY);
    

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void attack() override;
    bool isFacingRight() const { return m_facingRight; }

    

    void kickAttack();


    void airAttack();
    

    void jump();

    bool isJumping() const;


    void setParrying(bool parrying) override;
    

    std::unique_ptr<Entity> clone() const override;
    

    friend std::ostream& operator<<(std::ostream& os, const Player& player);

    enum class AttackType {
        PUNCH,
        KICK,
        AIR_ATTACK
    };



private:

    void initializeAnimations();
    

    void handleInput();
    

    static constexpr float MOVE_SPEED = 300.0f;
    static constexpr float JUMP_FORCE = 800.0f;
    static constexpr float GRAVITY = 2000.0f;
    static constexpr float ATTACK_DAMAGE = 20.0f;
    static constexpr float KICK_DAMAGE = 30.0f;
    static constexpr float AIR_ATTACK_DAMAGE = 40.0f;
    

    float m_attackCooldown;
    float m_attackTimer;
    

    float m_parryCooldown;
    float m_parryTimer;
    

    

    AttackType m_currentAttackType;
    

    

    void updateAnimation();
};

#endif