#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>


class Button {
public:

    Button(const std::string& text, const sf::Vector2f& position, 
           const sf::Vector2f& size, const sf::Font& font, 
           const std::function<void()>& callback);

    Button(const Button& other);

    Button& operator=(const Button& other);

    ~Button();

    void update(const sf::Vector2i& mousePosition);

    void render(sf::RenderWindow& window) const;

    void click();

    bool isMouseOver(const sf::Vector2i& mousePosition) const;

    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;

    void setSize(const sf::Vector2f& size);
    sf::Vector2f getSize() const;

    void setText(const std::string& text);
    std::string getText() const;

    void setIdleColor(const sf::Color& color);
    void setHoverColor(const sf::Color& color);
    void setPressColor(const sf::Color& color);
    void setTextColor(const sf::Color& color);

    void setCallback(const std::function<void()>& callback);

    friend std::ostream& operator<<(std::ostream& os, const Button& button);

private:

    sf::RectangleShape m_shape;

    sf::Text m_text;

    std::function<void()> m_callback;

    sf::Color m_idleColor;
    sf::Color m_hoverColor;
    sf::Color m_pressColor;

    enum class ButtonState {
        IDLE,
        HOVER,
        PRESS
    };
    
    ButtonState m_state;

    void centerText();
};

#endif