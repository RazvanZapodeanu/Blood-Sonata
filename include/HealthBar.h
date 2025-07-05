#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <SFML/Graphics.hpp>


class HealthBar {
public:

    HealthBar(float maxHealth, const sf::Vector2f& position, const sf::Vector2f& size);
    

    HealthBar(const HealthBar& other);
    

    HealthBar& operator=(const HealthBar& other);
    

    ~HealthBar();
    

    void update(float currentHealth);
    

    void render(sf::RenderWindow& window) const;
    

    void setPosition(float x, float y);
    

    sf::Vector2f getSize() const;
    

    friend std::ostream& operator<<(std::ostream& os, const HealthBar& healthBar);

private:

    sf::RectangleShape m_background;
    sf::RectangleShape m_foreground;
    

    float m_maxHealth;
    float m_currentHealth;
    

    sf::Vector2f m_position;
    sf::Vector2f m_size;
    

    sf::Color getHealthColor() const;
};

#endif