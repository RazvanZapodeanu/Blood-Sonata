#include "../include/Enemy.h"
#include "../include/SoundManager.h"
#include "../include/EffectManager.h"
#include <iostream>
#include <cmath>

std::mt19937 Enemy::s_randomEngine(static_cast<unsigned long>(std::time(nullptr)));

Enemy::Enemy(float posX, float posY, float health)
    : Entity("", posX, posY, 100.0f, 200.0f, health)
    , m_target(nullptr)
    , m_attackCooldown(2.0f)
    , m_attackTimer(0.0f)
    , m_parryCooldown(3.5f)
    , m_parryTimer(0.0f)
    , m_decisionTimer(0.0f)
    , m_decisionInterval(0.5f)
{
    initializeAnimations();
}

void Enemy::initializeAnimations() {

    std::vector<std::string> kickFrames = {
        "assets/sprites/kick.png",
        "assets/sprites/kick_1.png",
        "assets/sprites/kick_2.png"
    };
    auto kickAnim = std::make_unique<Animation>(kickFrames, 0.05f);
    kickAnim->setLooping(false);
    m_animations["kick"] = std::move(kickAnim);


    std::vector<std::string> punchFrames = {
        "assets/sprites/punch.png",
        "assets/sprites/punch_1.png",
        "assets/sprites/punch_2.png",
        "assets/sprites/punch_3.png",
        "assets/sprites/punch_4.png",
    };
    auto punchAnim = std::make_unique<Animation>(punchFrames, 0.05f);
    punchAnim->setLooping(false);
    m_animations["punch"] = std::move(punchAnim);

    std::vector<std::string> parryFrames = {
        "assets/sprites/parry.png"
    };
    auto parryAnim = std::make_unique<Animation>(parryFrames, 0.05f);
    parryAnim->setLooping(false);
    m_animations["parry"] = std::move(parryAnim);

    std::vector<std::string> deathFrames = {
        "assets/sprites/death.png",
        "assets/sprites/death_1.png",
        "assets/sprites/death_2.png",
        "assets/sprites/death_3.png",
        "assets/sprites/death_4.png"
    };
    auto deathAnim = std::make_unique<Animation>(deathFrames, 0.05f);
    deathAnim->setLooping(false);
    m_animations["death"] = std::move(deathAnim);

    std::vector<std::string> airattackFrames = {
        "assets/sprites/air_kick.png",
        "assets/sprites/air_kick_1.png",
        "assets/sprites/air_kick_2.png"
    };
    auto airattackAnim = std::make_unique<Animation>(airattackFrames, 0.05f);
    airattackAnim->setLooping(false);
    m_animations["airattack"] = std::move(airattackAnim);

    std::vector<std::string> idleFrames = {
        "assets/sprites/idle.png",
        "assets/sprites/idle_1.png",
        "assets/sprites/idle_2.png",
        "assets/sprites/idle_3.png"
    };
    auto idleAnim = std::make_unique<Animation>(idleFrames, 0.05f);
    idleAnim->setLooping(false);
    m_animations["idle"] = std::move(idleAnim);

    std::vector<std::string> winFrames = {
        "assets/sprites/win.png",
        "assets/sprites/win_1.png",
        "assets/sprites/win_2.png"
    };
    auto winAnim = std::make_unique<Animation>(winFrames, 0.05f);
    winAnim->setLooping(false);
    m_animations["win"] = std::move(winAnim);

    std::vector<std::string> jumpFrames = {
        "assets/sprites/jump.png",
        "assets/sprites/jump_1.png",
        "assets/sprites/jump_2.png",
        "assets/sprites/jump_3.png"
    };
    auto jumpAnim = std::make_unique<Animation>(jumpFrames, 0.05f);
    jumpAnim->setLooping(false);
    m_animations["jump"] = std::move(jumpAnim);

    std::vector<std::string> walkingFrames = {
        "assets/sprites/walking.png",
        "assets/sprites/walking_1.png",
        "assets/sprites/walking_2.png",
        "assets/sprites/walking_3.png",
        "assets/sprites/walking_4.png",
    };
    auto walkingAnim = std::make_unique<Animation>(walkingFrames, 0.05f);
    walkingAnim->setLooping(false);
    m_animations["walking"] = std::move(walkingAnim);

    std::vector<std::string> fallFrames = {
        "assets/sprites/fall.png",
        "assets/sprites/fall_1.png",
        "assets/sprites/fall.png"
    };
    auto fallAnim = std::make_unique<Animation>(fallFrames, 0.05f);
    fallAnim->setLooping(true);
    m_animations["walking"] = std::move(fallAnim);

    setAnimation("idle");
}
void Enemy::update(float deltaTime) {
    if (!m_isAlive) {

        if (m_currentAnimation) {
            m_currentAnimation->update(deltaTime);
        }
        return;
    }
    if (m_target && m_target->isAlive()) {
        sf::Vector2f targetPos = m_target->getPosition();


        if (targetPos.x > m_position.x) {
            m_facingRight = true;
            m_sprite.setScale(sf::Vector2f(1.0f, 1.0f));
        } else {
            m_facingRight = false;
            m_sprite.setScale(sf::Vector2f(-1.0f, 1.0f));
        }
    }

    if (m_attackTimer > 0.0f) {
        m_attackTimer -= deltaTime;
    }

    if (m_isAttacking && m_attackTimer <= 0.0f) {
        std::cout << "Enemy attack animation finished - back to idle!" << std::endl;
        m_isAttacking = false;
        setAnimation("idle");
    }

    if (m_parryTimer > 0.0f) {
        m_parryTimer -= deltaTime;
        

        if (m_parryTimer <= 0.0f && m_isParrying) {
            m_isParrying = false;
        }
    }
    

    m_decisionTimer -= deltaTime;
    if (m_decisionTimer <= 0.0f) {
        m_decisionTimer = m_decisionInterval;
        updateAI(deltaTime);
    }
    

    move(m_velocity.x * deltaTime, m_velocity.y * deltaTime);
    

    m_velocity.x = 0.0f;
    

    m_healthBar->update(m_health);
    m_healthBar->setPosition(m_position.x - m_size.x / 2.0f, m_position.y - m_size.y - 20.0f);
    

    updateAnimation();
    
    if (m_currentAnimation) {
        m_currentAnimation->update(deltaTime);
    }
}

void Enemy::updateAI(float /*deltaTime*/) {

    if (!m_target || !m_target->isAlive()) {
        return;
    }
    

    sf::Vector2f targetPos = m_target->getPosition();
    float distance = std::abs(targetPos.x - m_position.x);
    

    if (shouldParry() && m_parryTimer <= 0.0f && distance < 250.0f) {
        setParrying(true);
        m_parryTimer = m_parryCooldown;
        return;
    }
    

    if (distance < ATTACK_RANGE && shouldAttack() && canAttack()) {
        attack();
        return;
    }
    

    if (shouldMove()) {

        if (targetPos.x < m_position.x) {
            m_velocity.x = -MOVE_SPEED;
        } else {
            m_velocity.x = MOVE_SPEED;
        }
    }
}

bool Enemy::shouldMove() const {
    std::uniform_int_distribution<int> dist(0, 100);
    return dist(s_randomEngine) < 70;
}

bool Enemy::shouldAttack() const {
    std::uniform_int_distribution<int> dist(0, 100);
    return dist(s_randomEngine) < 60;
}

bool Enemy::shouldParry() const {
    std::uniform_int_distribution<int> dist(0, 100);
    return dist(s_randomEngine) < 15;
}

void Enemy::updateAnimation() {

    if (!m_isAlive || m_isAttacking || m_isParrying) {
        return;
    }
    
    if (m_velocity.x != 0.0f) {
        setAnimation("walking");
    } else {
        setAnimation("idle");
    }
}

void Enemy::render(sf::RenderWindow& window) {

    if (m_currentAnimation) {
        m_currentAnimation->applyToSprite(m_sprite);
    }
    
    window.draw(m_sprite);
    

    m_healthBar->render(window);
}

void Enemy::attack() {
    if (m_isAttacking || !m_isAlive) {
        return;
    }
    
    m_isAttacking = true;
    m_attackTimer = m_attackCooldown;
    

    setAnimation("punch");
    

    SoundManager::getInstance().playSound("enemy_attack");
    

    EffectManager::getInstance().createEffect("enemy_attack", sf::Vector2f(
        m_facingRight ? m_position.x + 50.0f : m_position.x - 50.0f,
        m_position.y - 100.0f
    ));
    

}

void Enemy::setTarget(Entity* target) {
    m_target = target;
}

bool Enemy::canAttack() const {
    return m_attackTimer <= 0.0f && m_isAlive && !m_isAttacking && !m_isParrying;
}

std::unique_ptr<Entity> Enemy::clone() const {
    return std::make_unique<Enemy>(*this);
}

std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
    os << "Enemy: " << static_cast<const Entity&>(enemy);
    return os;
}