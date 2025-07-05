#include "../include/Player.h"
#include "../include/SoundManager.h"
#include "../include/EffectManager.h"
#include <iostream>

Player::Player(float posX, float posY)
    : Entity("", posX, posY, 100.0f, 200.0f, 100.0f)
    , m_attackCooldown(0.5f)
    , m_attackTimer(0.0f)
    , m_parryCooldown(1.0f)
    , m_parryTimer(0.0f)
    , m_currentAttackType(AttackType::PUNCH)
{
    initializeAnimations();
}

void Player::initializeAnimations() {

    std::vector<std::string> kickFrames = {
        "assets/sprites/kick.png",
        "assets/sprites/kick_1.png",
        "assets/sprites/kick_2.png"
    };
    auto kickAnim = std::make_unique<Animation>(kickFrames, 0.15f);
    kickAnim->setLooping(false);
    m_animations["kick"] = std::move(kickAnim);


    std::vector<std::string> punchFrames = {
        "assets/sprites/punch.png",
        "assets/sprites/punch_1.png",
        "assets/sprites/punch_2.png",
        "assets/sprites/punch_3.png",
        "assets/sprites/punch_4.png",
    };
    auto punchAnim = std::make_unique<Animation>(punchFrames, 0.1f);
    punchAnim->setLooping(false);
    m_animations["punch"] = std::move(punchAnim);

    std::vector<std::string> parryFrames = {
        "assets/sprites/parry.png"
    };
    auto parryAnim = std::make_unique<Animation>(parryFrames, 0.1f);
    parryAnim->setLooping(false);
    m_animations["parry"] = std::move(parryAnim);

    std::vector<std::string> deathFrames = {
        "assets/sprites/death.png",
        "assets/sprites/death_1.png",
        "assets/sprites/death_2.png",
        "assets/sprites/death_3.png",
        "assets/sprites/death_4.png"
    };
    auto deathAnim = std::make_unique<Animation>(deathFrames, 0.3f);
    deathAnim->setLooping(false);
    m_animations["death"] = std::move(deathAnim);

    std::vector<std::string> airattackFrames = {
        "assets/sprites/air_kick.png",
        "assets/sprites/air_kick_1.png",
        "assets/sprites/air_kick_2.png"
    };
    auto airattackAnim = std::make_unique<Animation>(airattackFrames, 0.13f);
    airattackAnim->setLooping(false);
    m_animations["airattack"] = std::move(airattackAnim);

    std::vector<std::string> idleFrames = {
        "assets/sprites/idle.png",
        "assets/sprites/idle_1.png",
        "assets/sprites/idle_2.png",
        "assets/sprites/idle_3.png"
    };
    auto idleAnim = std::make_unique<Animation>(idleFrames, 0.2f);
    idleAnim->setLooping(true);
    m_animations["idle"] = std::move(idleAnim);

    std::vector<std::string> winFrames = {
        "assets/sprites/win.png",
        "assets/sprites/win_1.png",
        "assets/sprites/win_2.png"
    };
    auto winAnim = std::make_unique<Animation>(winFrames, 0.2f);
    winAnim->setLooping(false);
    m_animations["win"] = std::move(winAnim);

    std::vector<std::string> jumpFrames = {
        "assets/sprites/jump.png",
        "assets/sprites/jump_1.png",
        "assets/sprites/jump_2.png",
        "assets/sprites/jump_3.png"
    };
    auto jumpAnim = std::make_unique<Animation>(jumpFrames, 0.15f);
    jumpAnim->setLooping(true);
    m_animations["jump"] = std::move(jumpAnim);

    std::vector<std::string> walkingFrames = {
        "assets/sprites/walking.png",
        "assets/sprites/walking_1.png",
        "assets/sprites/walking_2.png",
        "assets/sprites/walking_3.png",
        "assets/sprites/walking_4.png",
    };
    auto walkingAnim = std::make_unique<Animation>(walkingFrames, 0.12f);
    walkingAnim->setLooping(true);
    m_animations["walking"] = std::move(walkingAnim);

    std::vector<std::string> fallFrames = {
        "assets/sprites/fall.png",
        "assets/sprites/fall_1.png",
        "assets/sprites/fall_2.png"
    };
    auto fallAnim = std::make_unique<Animation>(fallFrames, 0.2f);
    fallAnim->setLooping(true);
    m_animations["fall"] = std::move(fallAnim);

    setAnimation("idle");
}

void Player::update(float deltaTime) {
    if (!m_isAlive) {

        const float GROUND_LEVEL = 550.0f;
        if (m_position.y < GROUND_LEVEL) {
            m_position.y += 300.0f * deltaTime;
            m_sprite.setPosition(m_position);


            if (m_position.y >= GROUND_LEVEL) {
                m_position.y = GROUND_LEVEL;
                m_sprite.setPosition(m_position);
            }
        }

        if (m_currentAnimation) {
            m_currentAnimation->update(deltaTime);
        }
        return;
    }


    static float debugTimer = 0.0f;
    debugTimer += deltaTime;

    if (debugTimer > 1.0f) {
        std::cout << "START update() - velocity.x = " << m_velocity.x << std::endl;
        debugTimer = 0.0f;
    }


    handleInput();


    if (debugTimer > 1.0f) {
        std::cout << "AFTER handleInput() - velocity.x = " << m_velocity.x << std::endl;
    }


    if (m_attackTimer > 0.0f) {
        m_attackTimer -= deltaTime;
    }

    if (m_isAttacking && m_attackTimer <= 0.0f) {
        std::cout << "Attack timer expired - back to idle!" << std::endl;
        m_isAttacking = false;
        setAnimation("idle");
    }


    if (m_parryTimer > 0.0f) {
        m_parryTimer -= deltaTime;
        if (m_parryTimer <= 0.0f && m_isParrying) {
            std::cout << "PARRY TIMER FINISHED" << std::endl;
            m_isParrying = false;
        }
    }


    const float GROUND_LEVEL = 300.0f;
    if (m_position.y < GROUND_LEVEL) {
        m_velocity.y += GRAVITY * deltaTime;
    }


    if (debugTimer > 1.0f) {
        std::cout << "BEFORE move() - velocity(" << m_velocity.x << ", " << m_velocity.y << ")" << std::endl;
    }


    move(m_velocity.x * deltaTime, m_velocity.y * deltaTime);


    if (m_position.y >= GROUND_LEVEL) {
        m_position.y = GROUND_LEVEL;
        if (m_velocity.y > 0.0f) {
            m_velocity.y = 0.0f;
            if (m_isJumping) {
                std::cout << "PLAYER LANDED" << std::endl;
                m_isJumping = false;
                SoundManager::getInstance().playSound("land");
            }
        }
    }


    if (debugTimer > 1.0f) {
        std::cout << "AFTER move() - position(" << m_position.x << ", " << m_position.y
                  << "), velocity(" << m_velocity.x << ", " << m_velocity.y << ")" << std::endl;
    }


    m_healthBar->update(m_health);
    m_healthBar->setPosition(m_position.x - m_size.x / 2.0f, m_position.y - m_size.y - 20.0f);


    updateAnimation();

    if (m_currentAnimation) {
        m_currentAnimation->update(deltaTime);
    }


    m_velocity.x = 0.0f;
}



void Player::handleInput() {
    auto& inputManager = InputManager::getInstance();


    if (inputManager.isKeyPressed(sf::Keyboard::Scancode::A) ||
        inputManager.isKeyPressed(sf::Keyboard::Scancode::Left)) {
        m_velocity.x = -MOVE_SPEED;

        if (m_facingRight) {
            m_facingRight = false;
            m_sprite.setScale(sf::Vector2f(-1.0f, 1.0f));
        }
    }

    if (inputManager.isKeyPressed(sf::Keyboard::Scancode::D) ||
        inputManager.isKeyPressed(sf::Keyboard::Scancode::Right)) {
        m_velocity.x = MOVE_SPEED;

        if (!m_facingRight) {
            m_facingRight = true;
            m_sprite.setScale(sf::Vector2f(1.0f, 1.0f));
        }
    }


    static bool wasWPressed = false;
    static bool wasSpacePressed = false;
    static bool wasUpPressed = false;

    bool wPressed = inputManager.isKeyPressed(sf::Keyboard::Scancode::W);
    bool spacePressed = inputManager.isKeyPressed(sf::Keyboard::Scancode::Space);
    bool upPressed = inputManager.isKeyPressed(sf::Keyboard::Scancode::Up);

    if ((wPressed && !wasWPressed) || (spacePressed && !wasSpacePressed) || (upPressed && !wasUpPressed)) {
        if (!m_isJumping && m_position.y >= 295.0f) {
            jump();
        }
    }

    wasWPressed = wPressed;
    wasSpacePressed = spacePressed;
    wasUpPressed = upPressed;


    static bool wasJPressed = false;
    static bool wasKPressed = false;
    static bool wasLPressed = false;
    static bool wasFPressed = false;

    bool jPressed = inputManager.isKeyPressed(sf::Keyboard::Scancode::J);
    bool kPressed = inputManager.isKeyPressed(sf::Keyboard::Scancode::K);
    bool lPressed = inputManager.isKeyPressed(sf::Keyboard::Scancode::L);
    bool fPressed = inputManager.isKeyPressed(sf::Keyboard::Scancode::F);


    if (jPressed && !wasJPressed && m_attackTimer <= 0.0f) {
        m_currentAttackType = AttackType::PUNCH;
        attack();
    }


    if (kPressed && !wasKPressed && m_attackTimer <= 0.0f) {
        m_currentAttackType = AttackType::KICK;
        kickAttack();
    }


    if (lPressed && !wasLPressed && m_isJumping && m_attackTimer <= 0.0f) {
        m_currentAttackType = AttackType::AIR_ATTACK;
        airAttack();
    }


    if (fPressed && !wasFPressed && m_parryTimer <= 0.0f) {
        setParrying(true);
        m_parryTimer = m_parryCooldown;
    }

    wasJPressed = jPressed;
    wasKPressed = kPressed;
    wasLPressed = lPressed;
    wasFPressed = fPressed;
}



void Player::updateAnimation() {

    if (!m_isAlive) {
        setAnimation("death");
        return;
    }


    if (m_isAttacking && m_attackTimer > 0.0f) {
        return; // Doar dacă încă e în cooldown
    }

    if (m_isParrying) {
        setAnimation("parry");
        return;
    }


    std::string newAnimation;

    if (m_isJumping) {
        if (m_velocity.y < -50.0f) {
            newAnimation = "jump";
        } else {
            newAnimation = "fall";
        }
    } else if (std::abs(m_velocity.x) > 10.0f) {
        newAnimation = "walking";
    } else {
        newAnimation = "idle";
    }


    static std::string lastAnimation = "";
    if (newAnimation != lastAnimation) {
        std::cout << "FORCING ANIMATION CHANGE: " << lastAnimation << " -> " << newAnimation << std::endl;
        setAnimation(newAnimation);
        lastAnimation = newAnimation;
    }
    static float debugTimer = 0.0f;
    debugTimer += 0.016f;

    if (debugTimer > 1.0f) {
        std::cout << "PLAYER STATE: pos.y=" << m_position.y
                  << ", vel.x=" << m_velocity.x << ", vel.y=" << m_velocity.y
                  << ", isJumping=" << m_isJumping
                  << ", isAttacking=" << m_isAttacking
                  << ", currentAnim=" << newAnimation << std::endl;
        debugTimer = 0.0f;
    }


    if (newAnimation != lastAnimation) {
        std::cout << "ANIMATION CHANGE: " << lastAnimation << " -> " << newAnimation << std::endl;
        setAnimation(newAnimation);
        lastAnimation = newAnimation;
    }
}

void Player::render(sf::RenderWindow& window) {

    static float debugTimer = 0.0f;
    debugTimer += 0.016f;

    if (debugTimer > 2.0f) {
        std::cout << "=== RENDER DEBUG ===" << std::endl;
        std::cout << "Current animation exists: " << (m_currentAnimation ? "YES" : "NO") << std::endl;
        if (m_currentAnimation) {
            std::cout << "Animation is finished: " << (m_currentAnimation->isFinished() ? "YES" : "NO") << std::endl;
        }
        debugTimer = 0.0f;
    }


    if (m_currentAnimation) {
        m_currentAnimation->applyToSprite(m_sprite);
    }

    window.draw(m_sprite);
    m_healthBar->render(window);
}

void Player::attack() {
    if (m_isAttacking || !m_isAlive) {
        std::cout << "ATTACK BLOCKED: isAttacking=" << m_isAttacking << ", isAlive=" << m_isAlive << std::endl;
        return;
    }

    m_isAttacking = true;
    m_attackTimer = m_attackCooldown;

    std::cout << "ATTACK: Starting punch animation, timer set to " << m_attackTimer << std::endl;


    setAnimation("punch");


    SoundManager::getInstance().playSound("punch");
    EffectManager::getInstance().createEffect("punch", sf::Vector2f(
        m_facingRight ? m_position.x + 100.0f : m_position.x - 100.0f,
        m_position.y - 250.0f
    ));
}

void Player::kickAttack() {
    if (m_isAttacking || !m_isAlive) {
        std::cout << "KICK BLOCKED: isAttacking=" << m_isAttacking << ", isAlive=" << m_isAlive << std::endl;
        return;
    }

    m_isAttacking = true;
    m_attackTimer = m_attackCooldown * 1.2f;

    std::cout << "KICK: Starting kick animation, timer set to " << m_attackTimer << std::endl;

    setAnimation("kick");

    SoundManager::getInstance().playSound("kick");
    EffectManager::getInstance().createEffect("kick", sf::Vector2f(
        m_facingRight ? m_position.x + 110.0f : m_position.x - 110.0f,
        m_position.y - 0.0f
    ));
}


void Player::airAttack() {
    if (m_isAttacking || !m_isAlive || !m_isJumping) {
        std::cout << "AIR ATTACK BLOCKED: isAttacking=" << m_isAttacking
                  << ", isAlive=" << m_isAlive << ", isJumping=" << m_isJumping << std::endl;
        return;
    }

    m_isAttacking = true;
    m_attackTimer = m_attackCooldown;

    std::cout << "AIR ATTACK: Starting airattack animation, timer set to " << m_attackTimer << std::endl;

    setAnimation("airattack");

    SoundManager::getInstance().playSound("kick");
    EffectManager::getInstance().createEffect("air_attack", sf::Vector2f(
        m_facingRight ? m_position.x + 60.0f : m_position.x - 60.0f,
        m_position.y + 100.0f
    ));
}

void Player::jump() {
    std::cout << "Jump called! Position Y: " << m_position.y << ", isJumping: " << m_isJumping << std::endl;

    if (m_isJumping || !m_isAlive) {
        std::cout << "Jump BLOCKED - isJumping: " << m_isJumping << ", isAlive: " << m_isAlive << std::endl;
        return;
    }

    m_isJumping = true;
    m_velocity.y = -JUMP_FORCE;

    std::cout << "Jump SUCCESS! Velocity Y set to: " << m_velocity.y << std::endl;


    SoundManager::getInstance().playSound("jump");


    setAnimation("jump");
}


bool Player::isJumping() const {
    return m_isJumping;
}

void Player::setParrying(bool parrying) {
    if (!m_isAlive) {
        return;
    }

    Entity::setParrying(parrying);

    if (parrying) {

        SoundManager::getInstance().playSound("parry_start");
    }
}

std::unique_ptr<Entity> Player::clone() const {
    return std::make_unique<Player>(*this);
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << "Player: " << static_cast<const Entity&>(player);
    os << ", Attack Type: ";

    switch (player.m_currentAttackType) {
        case Player::AttackType::PUNCH:
            os << "Punch";
            break;
        case Player::AttackType::KICK:
            os << "Kick";
            break;
        case Player::AttackType::AIR_ATTACK:
            os << "Air Attack";
            break;
    }

    return os;
}