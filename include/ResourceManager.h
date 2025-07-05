#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>


class ResourceManager {
public:

    static ResourceManager& getInstance();
    

    void loadTexture(const std::string& id, const std::string& filePath);
    

    void loadFont(const std::string& id, const std::string& filePath);
    

    sf::Texture& getTexture(const std::string& id);
    

    sf::Font& getFont(const std::string& id);
    

    friend std::ostream& operator<<(std::ostream& os, const ResourceManager& resourceManager);

private:

    ResourceManager();
    

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    

    ~ResourceManager();
    

    std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::map<std::string, std::unique_ptr<sf::Font>> m_fonts;
};

#endif