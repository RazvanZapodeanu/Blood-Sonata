#include "../include/ResourceManager.h"
#include <iostream>
#include <stdexcept>

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {

}

void ResourceManager::loadTexture(const std::string& id, const std::string& filePath) {

    if (m_textures.find(id) != m_textures.end()) {
        std::cout << "Texture " << id << " already exists in manager" << std::endl;
        return;
    }
    

    auto texture = std::make_unique<sf::Texture>();
    

    if (!texture->loadFromFile(filePath)) {
        throw std::runtime_error("Couldn't load texture: " + filePath);
    }
    

    m_textures[id] = std::move(texture);
}

void ResourceManager::loadFont(const std::string& id, const std::string& filePath) {

    if (m_fonts.find(id) != m_fonts.end()) {
        std::cout << "Font " << id << " already exists in manager" << std::endl;
        return;
    }
    

    auto font = std::make_unique<sf::Font>();
    

    if (!font->openFromFile(filePath)) {
        throw std::runtime_error("Couldn't load font: " + filePath);
    }
    

    m_fonts[id] = std::move(font);
}

sf::Texture& ResourceManager::getTexture(const std::string& id) {

    auto it = m_textures.find(id);
    if (it == m_textures.end()) {
        throw std::runtime_error("Texture " + id + " doesn't exist in manager");
    }
    
    return *it->second;
}

sf::Font& ResourceManager::getFont(const std::string& id) {

    auto it = m_fonts.find(id);
    if (it == m_fonts.end()) {
        throw std::runtime_error("Font " + id + " doesn't exist in manager");
    }
    
    return *it->second;
}

std::ostream& operator<<(std::ostream& os, const ResourceManager& resourceManager) {
    os << "ResourceManager: textures=" << resourceManager.m_textures.size()
       << ", fonts=" << resourceManager.m_fonts.size();
    
    return os;
}