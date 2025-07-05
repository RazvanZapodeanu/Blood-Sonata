#include "../include/Entity.h"
#include "../include/SoundManager.h"
#include "../include/EffectManager.h"
#include <iostream>

Entity::Entity(const std::string& texturePath, float posX, float posY, float width, float height, float maxHealth)
    : m_sprite(m_texture)
, m_position(posX, posY)
      , m_size(width, height)
      , m_velocity(0.0f, 0.0f)
      , m_isAlive(true)
      , m_isParrying(false)
      , m_isAttacking(false)
      , m_isJumping(false)
      , m_health(maxHealth)
      , m_maxHealth(maxHealth)
      , m_facingRight(true) {
    initializeAnimation(texturePath);
    m_healthBar = std::make_unique<HealthBar>(m_maxHealth, sf::Vector2f(m_position.x, m_position.y - 20.0f),
                                              sf::Vector2f(width, 10.0f));
}

Entity::Entity(const Entity& other)
    : m_sprite(m_texture)
, m_position(other.m_position)
      , m_size(other.m_size)
      , m_velocity(other.m_velocity)
      , m_isAlive(other.m_isAlive)
      , m_isParrying(other.m_isParrying)
      , m_isAttacking(other.m_isAttacking)
      , m_isJumping(other.m_isJumping)
      , m_health(other.m_health)
      , m_maxHealth(other.m_maxHealth)
      , m_facingRight(other.m_facingRight) {

    m_texture = other.m_texture;
    m_sprite = other.m_sprite;
    m_sprite.setTexture(m_texture);


    m_healthBar = std::make_unique<HealthBar>(*other.m_healthBar);


    if (other.m_currentAnimation) {
        m_currentAnimation = std::make_unique<Animation>(*other.m_currentAnimation);
    }

    for (const auto &anim: other.m_animations) {
        m_animations[anim.first] = std::make_unique<Animation>(*anim.second);
    }
}

Entity& Entity::operator=(const Entity& other) {
    if (this != &other) {

        std::unique_ptr<Entity> temp=other.clone();

        
        std::swap(m_position, temp->m_position);
        std::swap(m_size, temp->m_size);
        std::swap(m_velocity, temp->m_velocity);
        std::swap(m_isAlive, temp->m_isAlive);
        std::swap(m_isParrying, temp->m_isParrying);
        std::swap(m_isAttacking, temp->m_isAttacking);
        std::swap(m_isJumping, temp->m_isJumping);
        std::swap(m_health, temp->m_health);
        std::swap(m_maxHealth, temp->m_maxHealth);
        std::swap(m_facingRight, temp->m_facingRight);
        std::swap(m_sprite, temp->m_sprite);
        std::swap(m_texture, temp->m_texture);
        std::swap(m_healthBar, temp->m_healthBar);
        std::swap(m_currentAnimation, temp->m_currentAnimation);
        std::swap(m_animations, temp->m_animations);
    }
    
    return *this;
}

Entity::~Entity() {

}

void Entity::initializeAnimation(const std::string& texturePath) {
    if (texturePath.empty()) {
        return;
    }

    if (!m_texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Couldn't load texture: " + texturePath);
    }

    m_sprite = sf::Sprite(m_texture);
    m_sprite.setPosition(m_position);



    m_sprite.setOrigin(sf::Vector2f(m_size.x / 2.0f, m_size.y));
}

void Entity::move(float x, float y) {
    m_position.x += x;
    m_position.y += y;

    m_sprite.setPosition(m_position);
    m_healthBar->setPosition(m_position.x - m_size.x / 2.0f, m_position.y - m_size.y - 20.0f);

}

bool Entity::checkCollision(const Entity& other) const {

    float normalRange = 30.0f;

    sf::FloatRect thisRect(
        sf::Vector2f(m_position.x - (m_size.x / 2.0f + normalRange),
                     m_position.y - m_size.y),
        sf::Vector2f(m_size.x + 2 * normalRange,
                     m_size.y)
    );

    sf::FloatRect otherRect(
        sf::Vector2f(other.m_position.x - other.m_size.x / 2.0f,
                     other.m_position.y - other.m_size.y),
        sf::Vector2f(other.m_size.x,
                     other.m_size.y)
    );

    return !(thisRect.position.x + thisRect.size.x < otherRect.position.x ||
             thisRect.position.x > otherRect.position.x + otherRect.size.x ||
             thisRect.position.y + thisRect.size.y < otherRect.position.y ||
             thisRect.position.y > otherRect.position.y + otherRect.size.y);
}
bool Entity::checkAttackRange(const Entity& other, const std::string& attackType, bool facingRight) const {

    float attackRange = 0.0f;

    if (attackType == "punch") {
        attackRange = 380.0f;
    } else if (attackType == "kick") {
        attackRange = 400.0f;
    } else if (attackType == "air_attack") {
        attackRange = 390.0f;
    }

    float totalRange = attackRange;


    sf::FloatRect attackZone;

    if (facingRight) {

        attackZone = sf::FloatRect(
            sf::Vector2f(m_position.x - m_size.x / 2.0f,
                         m_position.y - m_size.y),
            sf::Vector2f(m_size.x + totalRange,
                         m_size.y)
        );
    } else {

        attackZone = sf::FloatRect(
            sf::Vector2f(m_position.x - m_size.x / 2.0f - totalRange,
                         m_position.y - m_size.y),
            sf::Vector2f(m_size.x + totalRange,
                         m_size.y)
        );
    }


    sf::FloatRect otherRect(
        sf::Vector2f(other.m_position.x - other.m_size.x / 2.0f,
                     other.m_position.y - other.m_size.y),
        sf::Vector2f(other.m_size.x,
                     other.m_size.y)
    );


    std::cout << "Attack: " << attackType << ", Range: " << totalRange
              << ", Facing: " << (facingRight ? "RIGHT" : "LEFT") << std::endl;


    return !(attackZone.position.x + attackZone.size.x < otherRect.position.x ||
             attackZone.position.x > otherRect.position.x + otherRect.size.x ||
             attackZone.position.y + attackZone.size.y < otherRect.position.y ||
             attackZone.position.y > otherRect.position.y + otherRect.size.y);
}

void Entity::takeDamage(float damage, const std::string& attackType) {

    if (m_isParrying) {

        SoundManager::getInstance().playSound("parry");
        

        EffectManager::getInstance().createEffect("parry", m_position);
        return;
    }
    
    m_health -= damage;
    

    m_healthBar->update(m_health);

    if (attackType == "punch") {
        SoundManager::getInstance().playSound("punch_hit");
    } else {
        SoundManager::getInstance().playSound("kick_hit");
    }
    

    EffectManager::getInstance().createEffect("blood", m_position);
    

    if (m_health <= 0) {
        m_health = 0;
        die();
    }
}

void Entity::die() {
    m_isAlive = false;
    

    setAnimation("death");
    

    SoundManager::getInstance().playSound("death");
}

bool Entity::isAlive() const {
    return m_isAlive;
}

sf::Vector2f Entity::getPosition() const {
    return m_position;
}

void Entity::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
    
    m_sprite.setPosition(m_position);
    m_healthBar->setPosition(m_position.x - m_size.x / 2.0f, m_position.y - m_size.y - 20.0f);
}

sf::Vector2f Entity::getSize() const {
    return m_size;
}

bool Entity::isParrying() const {
    return m_isParrying;
}

void Entity::setParrying(bool parrying) {
    m_isParrying = parrying;
    
    if (m_isParrying) {
        setAnimation("parry");
    }
}

void Entity::setAnimation(const std::string& animName) {
    auto it = m_animations.find(animName);
    
    if (it != m_animations.end()) {
        m_currentAnimation = std::make_unique<Animation>(*it->second);
        m_currentAnimation->reset();


        if (m_currentAnimation) {
            m_currentAnimation->applyToSprite(m_sprite);
        }

        std::cout << "Animation " << animName << " applied to sprite!" << std::endl;
    }
}

void Entity::display(std::ostream& os) const {
    os << "Entity at position (" << m_position.x << ", " << m_position.y
       << "), size: (" << m_size.x << ", " << m_size.y
       << "), health: " << m_health << "/" << m_maxHealth
       << ", alive: " << (m_isAlive ? "yes" : "no");
}

std::ostream& operator<<(std::ostream& os, const Entity& entity) {
    entity.display(os);
    return os;
}