#include "../include/InputManager.h"
#include <iostream>

InputManager& InputManager::getInstance() {
    static InputManager instance;
    return instance;
}

InputManager::InputManager()
    : m_currentMousePosition(0, 0)
    , m_previousMousePosition(0, 0)
{
}

InputManager::~InputManager() {
}

void InputManager::processEvent(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        auto it = m_currentKeyState.find(keyPressed->scancode);
        bool wasAlreadyPressed = (it != m_currentKeyState.end()) ? it->second : false;

        if (!wasAlreadyPressed) {
            m_justPressedKeys.insert(keyPressed->scancode);
        }

        m_currentKeyState[keyPressed->scancode] = true;
    }
    else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {
        m_currentKeyState[keyReleased->scancode] = false;
    }


    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        m_currentButtonState[mouseButtonPressed->button] = true;
    }
    else if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        m_currentButtonState[mouseButtonReleased->button] = false;
    }
    else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_currentMousePosition = sf::Vector2i(mouseMoved->position);
    }
}

void InputManager::update() {

    m_previousKeyState = m_currentKeyState;


    m_previousButtonState = m_currentButtonState;


    m_previousMousePosition = m_currentMousePosition;


    m_justPressedKeys.clear();

}

bool InputManager::isKeyPressed(sf::Keyboard::Scancode scancode) const {

    auto it = m_currentKeyState.find(scancode);
    return (it != m_currentKeyState.end()) ? it->second : false;
}

bool InputManager::isKeyDown(sf::Keyboard::Scancode scancode) const {
    auto currentIt = m_currentKeyState.find(scancode);
    auto previousIt = m_previousKeyState.find(scancode);

    bool current = (currentIt != m_currentKeyState.end()) ? currentIt->second : false;
    bool previous = (previousIt != m_previousKeyState.end()) ? previousIt->second : false;

    bool result = current && !previous;


    if (scancode == sf::Keyboard::Scancode::W ||
        scancode == sf::Keyboard::Scancode::J ||
        scancode == sf::Keyboard::Scancode::K ||
        scancode == sf::Keyboard::Scancode::Space) {
        std::cout << "isKeyDown(" << static_cast<int>(scancode) << "): current=" << current
                  << ", previous=" << previous << ", result=" << result << std::endl;
        }

    return result;
}

bool InputManager::wasKeyJustPressed(sf::Keyboard::Scancode scancode) const {
    return m_justPressedKeys.count(scancode) > 0;
}


bool InputManager::isKeyReleased(sf::Keyboard::Scancode scancode) const {
    auto currentIt = m_currentKeyState.find(scancode);
    auto previousIt = m_previousKeyState.find(scancode);

    bool current = (currentIt != m_currentKeyState.end()) ? currentIt->second : false;
    bool previous = (previousIt != m_previousKeyState.end()) ? previousIt->second : false;

    return !current && previous;
}

bool InputManager::isButtonPressed(sf::Mouse::Button button) const {
    return sf::Mouse::isButtonPressed(button);
}

bool InputManager::isButtonDown(sf::Mouse::Button button) const {
    auto currentIt = m_currentButtonState.find(button);
    auto previousIt = m_previousButtonState.find(button);

    bool current = (currentIt != m_currentButtonState.end()) ? currentIt->second : false;
    bool previous = (previousIt != m_previousButtonState.end()) ? previousIt->second : false;

    return current && !previous;
}

bool InputManager::isButtonReleased(sf::Mouse::Button button) const {
    auto currentIt = m_currentButtonState.find(button);
    auto previousIt = m_previousButtonState.find(button);

    bool current = (currentIt != m_currentButtonState.end()) ? currentIt->second : false;
    bool previous = (previousIt != m_previousButtonState.end()) ? previousIt->second : false;

    return !current && previous;
}

sf::Vector2i InputManager::getMousePosition() const {
    return m_currentMousePosition;
}

sf::Vector2i InputManager::getMouseDelta() const {
    return m_currentMousePosition - m_previousMousePosition;
}

std::ostream& operator<<(std::ostream& os, const InputManager& inputManager) {
    os << "InputManager: mouse position=("
       << inputManager.m_currentMousePosition.x << ", "
       << inputManager.m_currentMousePosition.y << ")";

    return os;
}