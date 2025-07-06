#include "../include/UIManager.h"
#include "../include/Game.h"
#include "../include/ResourceManager.h"
#include <iostream>

UIManager::UIManager(sf::RenderWindow& window)
    : m_window(window)
    , m_activeMenu(MenuType::MAIN)
{

    m_font = ResourceManager::getInstance().getFont("main_font");
    

    for (int i = 0; i < 3; ++i) {
        MenuType type = static_cast<MenuType>(i);
        m_menus[type].isActive = false;
    }
}

UIManager::~UIManager() {

}

void UIManager::initialize() {

    initializeMainMenu();
    initializePauseMenu();
    initializeGameOverMenu();
    

    setActiveMenu(MenuType::MAIN);
}

void UIManager::initializeMainMenu() {

    addText(MenuType::MAIN, "STREET FIGHTER", sf::Vector2f(640.0f, 100.0f), 48, sf::Color::White);
    

    addButton(MenuType::MAIN, "START", sf::Vector2f(640.0f - 100.0f, 300.0f), sf::Vector2f(200.0f, 50.0f), 
             []() {
                 Game::getInstance().reset();
                 Game::getInstance().setGameState(GameState::PLAYING);
             });
    
    addButton(MenuType::MAIN, "EXIT", sf::Vector2f(640.0f - 100.0f, 400.0f), sf::Vector2f(200.0f, 50.0f), 
             []() {
                 Game::getInstance().exit();
             });
}

void UIManager::initializePauseMenu() {

    addText(MenuType::PAUSE, "PAUSE", sf::Vector2f(640.0f, 100.0f), 48, sf::Color::White);
    

    addButton(MenuType::PAUSE, "RESUME", sf::Vector2f(640.0f - 100.0f, 300.0f), sf::Vector2f(200.0f, 50.0f), 
             []() {
                 Game::getInstance().setGameState(GameState::PLAYING);
             });
    
    addButton(MenuType::PAUSE, "RESET", sf::Vector2f(640.0f - 100.0f, 400.0f), sf::Vector2f(200.0f, 50.0f), 
             []() {
                 Game::getInstance().reset();
                 Game::getInstance().setGameState(GameState::PLAYING);
             });
    
    addButton(MenuType::PAUSE, "EXIT", sf::Vector2f(640.0f - 100.0f, 500.0f), sf::Vector2f(200.0f, 50.0f), 
             []() {
                 Game::getInstance().setGameState(GameState::MENU);
             });
}

void UIManager::initializeGameOverMenu() {

    addText(MenuType::GAME_OVER, "GAME OVER", sf::Vector2f(640.0f, 100.0f), 48, sf::Color::Red);
    

    addButton(MenuType::GAME_OVER, "RETRY", sf::Vector2f(640.0f - 100.0f, 300.0f), sf::Vector2f(200.0f, 50.0f), 
             []() {
                 Game::getInstance().reset();
                 Game::getInstance().setGameState(GameState::PLAYING);
             });
    
    addButton(MenuType::GAME_OVER, "EXIT", sf::Vector2f(640.0f - 100.0f, 400.0f), sf::Vector2f(200.0f, 50.0f), 
             []() {
                 Game::getInstance().setGameState(GameState::MENU);
             });
}

void UIManager::update(float deltaTime) {

    sf::Vector2i mousePosition = sf::Mouse::getPosition(m_window);
    

    for (auto& menu : m_menus) {
        if (menu.second.isActive) {
            for (auto& button : menu.second.buttons) {
                button->update(mousePosition);
            }
        }
    }
}

void UIManager::render() {

    for (auto& menu : m_menus) {
        if (menu.second.isActive) {

            for (auto& button : menu.second.buttons) {
                button->render(m_window);
            }
            

            for (auto& text : menu.second.texts) {
                m_window.draw(text);
            }
        }
    }
}

void UIManager::setActiveMenu(MenuType menuType) {

    for (auto& menu : m_menus) {
        menu.second.isActive = false;
    }
    

    m_menus[menuType].isActive = true;
    m_activeMenu = menuType;
}

bool UIManager::isMenuActive(MenuType menuType) const {
    auto it = m_menus.find(menuType);
    if (it != m_menus.end()) {
        return it->second.isActive;
    }
    
    return false;
}

void UIManager::handleInput(const sf::Event& event) {

    if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left) {

            sf::Vector2i mousePosition(mouseButtonReleased->position);


            for (auto& menu : m_menus) {
                if (menu.second.isActive) {
                    for (auto& button : menu.second.buttons) {
                        if (button->isMouseOver(mousePosition)) {
                            button->click();
                        }
                    }
                }
            }
        }
    }
}

void UIManager::addButton(MenuType menuType, const std::string& text, const sf::Vector2f& position,
                           const sf::Vector2f& size, const std::function<void()>& callback) {

    auto button = std::make_unique<Button>(text, position, size, m_font, callback);


    button->setIdleColor(sf::Color(100, 100, 100));
    button->setHoverColor(sf::Color(150, 150, 150));
    button->setPressColor(sf::Color(50, 50, 50));
    button->setTextColor(sf::Color::White);


    m_menus[menuType].buttons.push_back(std::move(button));
}

void UIManager::addText(MenuType menuType, const std::string& text, const sf::Vector2f& position,
                         unsigned int characterSize, const sf::Color& color) {

    sf::Text newText(m_font, text, characterSize);
    newText.setFillColor(color);


    sf::FloatRect textBounds = newText.getLocalBounds();
    newText.setOrigin(sf::Vector2f(textBounds.size.x / 2.0f, textBounds.size.y / 2.0f));
    newText.setPosition(position);
    

    m_menus[menuType].texts.push_back(newText);
}

std::ostream& operator<<(std::ostream& os, const UIManager& uiManager) {
    os << "UIManager: active menu=";
    
    switch (uiManager.m_activeMenu) {
        case MenuType::MAIN:
            os << "MAIN";
            break;
        case MenuType::PAUSE:
            os << "PAUSE";
            break;
        case MenuType::GAME_OVER:
            os << "GAME_OVER";
            break;
    }
    
    return os;
}