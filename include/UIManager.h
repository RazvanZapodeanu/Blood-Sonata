#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <functional>
#include <map>

#include "Button.h"


enum class MenuType {
    MAIN,
    PAUSE,
    GAME_OVER
};


class UIManager {
public:

    explicit UIManager(sf::RenderWindow& window);
    

    ~UIManager();
    

    void initialize();
    

    void update(float deltaTime);
    

    void render();
    

    void setActiveMenu(MenuType menuType);
    

    bool isMenuActive(MenuType menuType) const;
    

    void handleInput(const sf::Event& event);
    

    void addButton(MenuType menuType, const std::string& text, const sf::Vector2f& position, 
                   const sf::Vector2f& size, const std::function<void()>& callback);
    

    void addText(MenuType menuType, const std::string& text, const sf::Vector2f& position, 
                 unsigned int characterSize, const sf::Color& color);
    

    friend std::ostream& operator<<(std::ostream& os, const UIManager& uiManager);

private:

    sf::RenderWindow& m_window;
    

    struct Menu {
        std::vector<std::unique_ptr<Button>> buttons;
        std::vector<sf::Text> texts;
        bool isActive;
    };
    

    std::map<MenuType, Menu> m_menus;
    

    MenuType m_activeMenu;
    

    sf::Font m_font;
    

    void initializeMainMenu();
    void initializePauseMenu();
    void initializeGameOverMenu();
};

#endif