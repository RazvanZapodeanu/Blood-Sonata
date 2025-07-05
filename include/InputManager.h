#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <unordered_set>


class InputManager {
public:

    static InputManager& getInstance();


    void processEvent(const sf::Event& event);


    void update();


    bool isKeyPressed(sf::Keyboard::Scancode scancode) const;
    bool isKeyDown(sf::Keyboard::Scancode scancode) const;
    bool isKeyReleased(sf::Keyboard::Scancode scancode) const;
    bool wasKeyJustPressed(sf::Keyboard::Scancode scancode) const;


    bool isButtonPressed(sf::Mouse::Button button) const;
    bool isButtonDown(sf::Mouse::Button button) const;
    bool isButtonReleased(sf::Mouse::Button button) const;


    sf::Vector2i getMousePosition() const;
    sf::Vector2i getMouseDelta() const;


    friend std::ostream& operator<<(std::ostream& os, const InputManager& inputManager);

private:

    InputManager();


    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;


    ~InputManager();


    std::map<sf::Keyboard::Scancode, bool> m_currentKeyState;
    std::map<sf::Keyboard::Scancode, bool> m_previousKeyState;
    std::unordered_set<sf::Keyboard::Scancode> m_justPressedKeys;


    std::map<sf::Mouse::Button, bool> m_currentButtonState;
    std::map<sf::Mouse::Button, bool> m_previousButtonState;


    sf::Vector2i m_currentMousePosition;
    sf::Vector2i m_previousMousePosition;
};

#endif