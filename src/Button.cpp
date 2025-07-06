#include "../include/Button.h"
#include "../include/SoundManager.h"
#include <iostream>

Button::Button(const std::string& text, const sf::Vector2f& position, 
               const sf::Vector2f& size, const sf::Font& font, 
               const std::function<void()>& callback)
    : m_shape()
    , m_text(font,text,24)
    , m_callback(callback)
    , m_idleColor(sf::Color(100, 100, 100))
    , m_hoverColor(sf::Color(150, 150, 150))
    , m_pressColor(sf::Color(50, 50, 50))
    , m_state(ButtonState::IDLE)
{

    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(m_idleColor);
    m_shape.setOutlineThickness(1.0f);
    m_shape.setOutlineColor(sf::Color::Black);


    m_text.setFont(font);
    m_text.setString(text);
    m_text.setFillColor(sf::Color::White);
    m_text.setCharacterSize(24);


    centerText();
}

Button::Button(const Button& other)
    : m_shape(other.m_shape)
    , m_text(other.m_text)
    , m_callback(other.m_callback)
    , m_idleColor(other.m_idleColor)
    , m_hoverColor(other.m_hoverColor)
    , m_pressColor(other.m_pressColor)
    , m_state(other.m_state)
{
}

Button& Button::operator=(const Button& other) {
    if (this != &other) {
        m_shape = other.m_shape;
        m_text = other.m_text;
        m_callback = other.m_callback;
        m_idleColor = other.m_idleColor;
        m_hoverColor = other.m_hoverColor;
        m_pressColor = other.m_pressColor;
        m_state = other.m_state;
    }
    
    return *this;
}

Button::~Button() {

}

void Button::update(const sf::Vector2i& mousePosition) {

    m_state = ButtonState::IDLE;
    

    if (isMouseOver(mousePosition)) {

        m_state = ButtonState::HOVER;
        

        if (isButtonPressed(sf::Mouse::Button::Left)) {

            m_state = ButtonState::PRESS;
        }
    }
    

    switch (m_state) {
        case ButtonState::IDLE:
            m_shape.setFillColor(m_idleColor);
            break;
        case ButtonState::HOVER:
            m_shape.setFillColor(m_hoverColor);
            break;
        case ButtonState::PRESS:
            m_shape.setFillColor(m_pressColor);
            break;
    }
}

void Button::render(sf::RenderWindow& window) const {

    window.draw(m_shape);
    

    window.draw(m_text);
}

void Button::click() {

    SoundManager::getInstance().playSound("button_click");
    

    if (m_callback) {
        m_callback();
    }
}

bool Button::isMouseOver(const sf::Vector2i& mousePosition) const {

    sf::Vector2f mousePos(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
    

    return (mousePos.x >= m_shape.getPosition().x && 
            mousePos.x <= m_shape.getPosition().x + m_shape.getSize().x &&
            mousePos.y >= m_shape.getPosition().y &&
            mousePos.y <= m_shape.getPosition().y + m_shape.getSize().y);
}

void Button::setPosition(const sf::Vector2f& position) {
    m_shape.setPosition(position);
    

    centerText();
}

sf::Vector2f Button::getPosition() const {
    return m_shape.getPosition();
}

void Button::setSize(const sf::Vector2f& size) {
    m_shape.setSize(size);
    

    centerText();
}

sf::Vector2f Button::getSize() const {
    return m_shape.getSize();
}

void Button::setText(const std::string& text) {
    m_text.setString(text);
    

    centerText();
}

std::string Button::getText() const {
    return m_text.getString();
}

void Button::setIdleColor(const sf::Color& color) {
    m_idleColor = color;
}

void Button::setHoverColor(const sf::Color& color) {
    m_hoverColor = color;
}

void Button::setPressColor(const sf::Color& color) {
    m_pressColor = color;
}

void Button::setTextColor(const sf::Color& color) {
    m_text.setFillColor(color);
}

void Button::setCallback(const std::function<void()>& callback) {
    m_callback = callback;
}

void Button::centerText() {

    sf::FloatRect buttonBounds = m_shape.getGlobalBounds();
    

    sf::FloatRect textBounds = m_text.getGlobalBounds();
    

    float xPos = buttonBounds.position.x + (buttonBounds.size.x - textBounds.size.x) / 2.0f;
    float yPos = buttonBounds.position.y + (buttonBounds.size.y - textBounds.size.y) / 2.0f - 5.0f; // -5 pentru ajustare
    

    m_text.setPosition(sf::Vector2f(xPos, yPos));
}

std::ostream& operator<<(std::ostream& os, const Button& button) {
    os << "Button: text=" << button.m_text.getString().toAnsiString()
       << ", position=(" << button.m_shape.getPosition().x << "," << button.m_shape.getPosition().y << ")"
       << ", size=(" << button.m_shape.getSize().x << "," << button.m_shape.getSize().y << ")";
    
    return os;
}