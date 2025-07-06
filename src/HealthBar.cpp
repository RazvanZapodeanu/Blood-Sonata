#include "../include/HealthBar.h"
#include <iostream>

HealthBar::HealthBar(float maxHealth, const sf::Vector2f& position, const sf::Vector2f& size)
    : m_maxHealth(maxHealth)
    , m_currentHealth(maxHealth)
    , m_position(position)
    , m_size(size)
{

    m_background.setSize(size);
    m_background.setFillColor(sf::Color(50, 50, 50));
    m_background.setPosition(position);
    m_background.setOutlineThickness(1.0f);
    m_background.setOutlineColor(sf::Color::Black);
    

    m_foreground.setSize(size);
    m_foreground.setFillColor(sf::Color::Green);
    m_foreground.setPosition(position);
}

HealthBar::HealthBar(const HealthBar& other)
    : m_background(other.m_background)
    , m_foreground(other.m_foreground)
    ,m_maxHealth(other.m_maxHealth)
    , m_currentHealth(other.m_currentHealth)
    , m_position(other.m_position)
    , m_size(other.m_size)

{
}

HealthBar& HealthBar::operator=(const HealthBar& other) {
    if (this != &other) {
        m_maxHealth = other.m_maxHealth;
        m_currentHealth = other.m_currentHealth;
        m_position = other.m_position;
        m_size = other.m_size;
        m_background = other.m_background;
        m_foreground = other.m_foreground;
    }
    
    return *this;
}

HealthBar::~HealthBar() {

}

void HealthBar::update(float currentHealth) {
    m_currentHealth = std::max(0.0f, std::min(currentHealth, m_maxHealth));
    

    float width = (m_currentHealth / m_maxHealth) * m_size.x;
    m_foreground.setSize(sf::Vector2f(width, m_size.y));
    

    m_foreground.setFillColor(getHealthColor());
}

void HealthBar::render(sf::RenderWindow& window) const {
    window.draw(m_background);
    window.draw(m_foreground);
}

void HealthBar::setPosition(float x, float y) {
    m_position.x = x;
    m_position.y = y;
    
    m_background.setPosition(m_position);
    m_foreground.setPosition(m_position);
}

sf::Vector2f HealthBar::getSize() const {
    return m_size;
}

sf::Color HealthBar::getHealthColor() const {

    float healthPercent = m_currentHealth / m_maxHealth;
    
    if (healthPercent > 0.6f) {

        return sf::Color::Green;
    } else if (healthPercent > 0.3f) {

        return sf::Color::Yellow;
    } else {

        return sf::Color::Red;
    }
}

std::ostream& operator<<(std::ostream& os, const HealthBar& healthBar) {
    os << "HealthBar: " << healthBar.m_currentHealth << "/" << healthBar.m_maxHealth
       << " (" << (healthBar.m_currentHealth / healthBar.m_maxHealth * 100.0f) << "%)";
    
    return os;
}