#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "Animation.h"
#include "HealthBar.h"


class Enemy;
class Entity {
public:

    Entity(const std::string& texturePath, float posX, float posY, float width, float height, float maxHealth);
    

    Entity(const Entity& other);
    

    Entity& operator=(const Entity& other);
    

    virtual ~Entity();
    

    virtual std::unique_ptr<Entity> clone() const = 0;
    

    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void attack() = 0;
    virtual void takeDamage(float damage, const std::string& attackType = "default");
    virtual void die();
    virtual bool isAlive() const;
    bool checkAttackRange(const Entity& other, const std::string& attackType = "punch", bool facingRight = true) const;
    

    void move(float x, float y);
    

    bool checkCollision(const Entity& other) const;
    

    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
    

    sf::Vector2f getSize() const;
    

    virtual bool isParrying() const;
    

    virtual void setParrying(bool parrying);

    void print(std::ostream& os) const {
        display(os);
    }
    

    friend std::ostream& operator<<(std::ostream& os, const Entity& entity);

protected:

    void display(std::ostream& os) const;
    

    void initializeAnimation(const std::string& texturePath);



    sf::Sprite m_sprite;
    sf::Texture m_texture;
    

    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Vector2f m_velocity;
    

    bool m_isAlive;
    bool m_isParrying;
    bool m_isAttacking;
    bool m_isJumping;
    

    float m_health;
    float m_maxHealth;
    std::unique_ptr<HealthBar> m_healthBar;
    

    bool m_facingRight;
    

    std::unique_ptr<Animation> m_currentAnimation;
    std::map<std::string, std::unique_ptr<Animation>> m_animations;
    

    void setAnimation(const std::string& animName);
};

#endif